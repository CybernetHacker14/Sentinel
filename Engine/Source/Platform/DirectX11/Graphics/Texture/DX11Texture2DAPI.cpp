#include "stpch.h"
#include "Platform/DirectX11/Graphics/Core/DX11Common.h"
#include "Platform/DirectX11/Graphics/Texture/DX11Texture2DAPI.h"

#include "Platform/DirectX11/Graphics/Device/DX11ContextAPI.h"

#include <stb_image.h>

namespace Sentinel {
    STL::unordered_map<TextureWrapMode, D3D11_TEXTURE_ADDRESS_MODE> s_WrapModeDescTable = {
        {TextureWrapMode::REPEAT, D3D11_TEXTURE_ADDRESS_WRAP},
        {TextureWrapMode::CLAMP, D3D11_TEXTURE_ADDRESS_CLAMP},
        {TextureWrapMode::MIRROR, D3D11_TEXTURE_ADDRESS_MIRROR}};

    DX11Texture2DAPI::_init DX11Texture2DAPI::_initializer;

    void DX11Texture2DAPI::Bind(Texture2DData* dataObject, UInt32 slot, const ShaderType shaderType) {
        ID3D11DeviceContext* context =
            DX11ContextAPI::GetNativeContext(ContextAPI::Cast<DX11ContextData>(dataObject->Context));
        DX11Texture2DData* texture = Texture2DAPI::Cast<DX11Texture2DData>(dataObject);
        switch (shaderType) {
            case ShaderType::VERTEX:
                context->VSSetShaderResources(slot, 1, &(texture->m_ResourceView));
                context->VSSetSamplers(slot, 1, &(texture->m_SamplerState));
                break;
            case ShaderType::PIXEL:
                context->PSSetShaderResources(slot, 1, &(texture->m_ResourceView));
                context->PSSetSamplers(slot, 1, &(texture->m_SamplerState));
                break;
            case ShaderType::COMPUTE:
                context->CSSetShaderResources(slot, 1, &(texture->m_ResourceView));
                context->CSSetSamplers(slot, 1, &(texture->m_SamplerState));
                break;
            case ShaderType::NONE: ST_ENGINE_ASSERT(false, "Invalid Shader type"); break;
        }
    }

    void DX11Texture2DAPI::Unbind(Texture2DData* dataObject) {
        ID3D11DeviceContext* context =
            DX11ContextAPI::GetNativeContext(ContextAPI::Cast<DX11ContextData>(dataObject->Context));
        ID3D11SamplerState* nullSampler = nullptr;
        ID3D11ShaderResourceView* nullSRV = nullptr;
        DX11Texture2DData* texture = Texture2DAPI::Cast<DX11Texture2DData>(dataObject);
        UInt32 slot = Texture2DAPI::GetBindSlot(dataObject);
        switch (Texture2DAPI::GetBindType(dataObject)) {
            case ShaderType::VERTEX:
                context->VSSetShaderResources(slot, 1, &(texture->m_ResourceView));
                context->VSSetSamplers(slot, 1, &(texture->m_SamplerState));
                break;
            case ShaderType::PIXEL:
                context->PSSetShaderResources(slot, 1, &(texture->m_ResourceView));
                context->PSSetSamplers(slot, 1, &(texture->m_SamplerState));
                break;
            case ShaderType::COMPUTE:
                context->CSSetShaderResources(slot, 1, &(texture->m_ResourceView));
                context->CSSetSamplers(slot, 1, &(texture->m_SamplerState));
                break;
            case ShaderType::NONE: ST_ENGINE_ASSERT(false, "Invalid Shader type"); break;
        }
    }

    void DX11Texture2DAPI::Clean(Texture2DData* dataObject) {
        DX11Texture2DData* texture = Texture2DAPI::Cast<DX11Texture2DData>(dataObject);
        if (texture->m_ResourceView) {
            texture->m_ResourceView->Release();
            texture->m_ResourceView = nullptr;
        }

        if (texture->m_SamplerState) {
            texture->m_SamplerState->Release();
            texture->m_SamplerState = nullptr;
        }

        free(texture->m_TexturePixels);
    }

    void DX11Texture2DAPI::Load(DX11Texture2DData* dataObject) {
        ID3D11Texture2D* texture2D = nullptr;

        D3D11_TEXTURE2D_DESC textureDescription;
        SecureZeroMemory(&textureDescription, sizeof(textureDescription));
        textureDescription.ArraySize = 1;
        textureDescription.SampleDesc.Count = 1;
        textureDescription.SampleDesc.Quality = 0;
        textureDescription.Usage = D3D11_USAGE_DEFAULT;
        textureDescription.CPUAccessFlags = 0;
        textureDescription.BindFlags = D3D11_BIND_SHADER_RESOURCE;
        textureDescription.MiscFlags = dataObject->m_Settings.GenerateMipMaps ? D3D11_RESOURCE_MISC_GENERATE_MIPS : 0;

        // Set Format
        int width, height, channels;
        const char* path = dataObject->m_Settings.TextureFilepath.c_str();
        dataObject->m_HDR = stbi_is_hdr(path) ? true : false;
        dataObject->m_TexturePixels = static_cast<void*>(stbi_load(path, &width, &height, &channels, 4));

        if (dataObject->m_TexturePixels == nullptr) {
            ST_ENGINE_ERROR("Failed to load image at path : {0}", path);
            return;
        }

        textureDescription.Format = dataObject->m_HDR ? DXGI_FORMAT_R32G32B32A32_FLOAT
                                                      : (dataObject->m_Settings.sRGB ? DXGI_FORMAT_R8G8B8A8_UNORM_SRGB
                                                                                     : DXGI_FORMAT_R8G8B8A8_UNORM);

        dataObject->m_Width = width;
        dataObject->m_Height = height;
        stbi_set_flip_vertically_on_load(false);
        // \Set Format

        textureDescription.Width = dataObject->m_Width;
        textureDescription.Height = dataObject->m_Height;
        textureDescription.MipLevels = dataObject->m_HDR ? 1 : 0;

        if (dataObject->m_Settings.GenerateMipMaps) textureDescription.BindFlags |= D3D11_BIND_RENDER_TARGET;

        DX11ContextData* context = ContextAPI::Cast<DX11ContextData>(dataObject->Context);

        DX11ContextAPI::GetDevice(context)->CreateTexture2D(&textureDescription, nullptr, &texture2D);

        UInt32 rowPitch = dataObject->m_Width * 4 * (dataObject->m_HDR ? sizeof(float) : sizeof(unsigned char));
        DX11ContextAPI::GetNativeContext(context)->UpdateSubresource(
            texture2D, 0, nullptr, dataObject->m_TexturePixels, rowPitch, 0);

        D3D11_SHADER_RESOURCE_VIEW_DESC viewDescription;
        SecureZeroMemory(&viewDescription, sizeof(viewDescription));
        viewDescription.Format = textureDescription.Format;
        viewDescription.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
        viewDescription.Texture2D.MostDetailedMip = 0;
        viewDescription.Texture2D.MipLevels = dataObject->m_Settings.GenerateMipMaps ? -1 : 1;

        DX11ContextAPI::GetDevice(context)->CreateShaderResourceView(
            texture2D, &viewDescription, &(dataObject->m_ResourceView));

        if (dataObject->m_Settings.GenerateMipMaps)
            DX11ContextAPI::GetNativeContext(context)->GenerateMips(dataObject->m_ResourceView);

        {
            D3D11_SAMPLER_DESC samplerDesc;
            SecureZeroMemory(&samplerDesc, sizeof(samplerDesc));
            samplerDesc.Filter = D3D11_FILTER_ANISOTROPIC;
            samplerDesc.AddressU = s_WrapModeDescTable[dataObject->m_Settings.WrapMode];
            samplerDesc.AddressV = s_WrapModeDescTable[dataObject->m_Settings.WrapMode];
            samplerDesc.AddressW = s_WrapModeDescTable[dataObject->m_Settings.WrapMode];
            samplerDesc.MaxAnisotropy = D3D11_REQ_MAXANISOTROPY;
            samplerDesc.MipLODBias = 0.0f;
            samplerDesc.MinLOD = 0.0f;
            samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

            DX11ContextAPI::GetDevice(context)->CreateSamplerState(&samplerDesc, &(dataObject->m_SamplerState));
        }
    }
}  // namespace Sentinel