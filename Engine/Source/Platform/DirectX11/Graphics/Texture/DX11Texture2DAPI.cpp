#include "stpch.h"

#ifdef ST_RENDERER_DX11

    #include "Sentinel/Graphics/Texture/Texture2DAPI.h"
    #include "Sentinel/Graphics/Device/ContextAPI.h"

    #include "Platform/DirectX11/Graphics/Core/DX11Common.h"

    #include <stb_image.h>

namespace Sentinel {
    STL::unordered_map<TextureWrapMode, D3D11_TEXTURE_ADDRESS_MODE> s_WrapModeDescTable = {
        {TextureWrapMode::REPEAT, D3D11_TEXTURE_ADDRESS_WRAP},
        {TextureWrapMode::CLAMP, D3D11_TEXTURE_ADDRESS_CLAMP},
        {TextureWrapMode::MIRROR, D3D11_TEXTURE_ADDRESS_MIRROR}};

    Texture2DData* Sentinel::Texture2DAPI::CreateTexture2DData(
        PoolAllocator<Texture2DData>& allocator, ContextData* context, const Texture2DDataImportSettings& settings) {
        Texture2DData* texObject = allocator.New();
        texObject->Context = context;
        texObject->m_Settings = settings;
        texObject->m_BindType = ShaderType::NONE;
        Load(texObject);
        return texObject;
    }

    Texture2DData* Texture2DAPI::CreateTexture2DData(
        PoolAllocator<Texture2DData>& allocator,
        ContextData* context,
        const Texture2DDataImportSettings& settings,
        UInt8* pixelData,
        const UInt32 width,
        const UInt32 height,
        const UInt8 channels) {
        Texture2DData* texObject = allocator.New();
        texObject->m_TexturePixels = pixelData;
        texObject->Context = context;
        texObject->m_Settings = settings;
        texObject->m_BindType = ShaderType::NONE;
        Load(texObject, width, height, channels);
        return texObject;
    }

    void Texture2DAPI::Bind(Texture2DData* dataObject, UInt32 slot, const ShaderType shaderType) {
        ID3D11DeviceContext* context = ContextAPI::GetNativeContext(dataObject->Context);
        switch (shaderType) {
            case ShaderType::VERTEX:
                context->VSSetShaderResources(slot, 1, &(dataObject->m_ResourceView));
                context->VSSetSamplers(slot, 1, &(dataObject->m_SamplerState));
                break;
            case ShaderType::PIXEL:
                context->PSSetShaderResources(slot, 1, &(dataObject->m_ResourceView));
                context->PSSetSamplers(slot, 1, &(dataObject->m_SamplerState));
                break;
            case ShaderType::COMPUTE:
                context->CSSetShaderResources(slot, 1, &(dataObject->m_ResourceView));
                context->CSSetSamplers(slot, 1, &(dataObject->m_SamplerState));
                break;
            case ShaderType::NONE: ST_ENGINE_ASSERT(false, "Invalid Shader type"); break;
        }
    }

    void Texture2DAPI::Unbind(Texture2DData* dataObject) {
        ID3D11DeviceContext* context = ContextAPI::GetNativeContext(dataObject->Context);
        ID3D11SamplerState* nullSampler = nullptr;
        ID3D11ShaderResourceView* nullSRV = nullptr;
        UInt32 slot = Texture2DAPI::GetBindSlot(dataObject);
        switch (Texture2DAPI::GetBindType(dataObject)) {
            case ShaderType::VERTEX:
                context->VSSetShaderResources(slot, 1, &(dataObject->m_ResourceView));
                context->VSSetSamplers(slot, 1, &(dataObject->m_SamplerState));
                break;
            case ShaderType::PIXEL:
                context->PSSetShaderResources(slot, 1, &(dataObject->m_ResourceView));
                context->PSSetSamplers(slot, 1, &(dataObject->m_SamplerState));
                break;
            case ShaderType::COMPUTE:
                context->CSSetShaderResources(slot, 1, &(dataObject->m_ResourceView));
                context->CSSetSamplers(slot, 1, &(dataObject->m_SamplerState));
                break;
            case ShaderType::NONE: ST_ENGINE_ASSERT(false, "Invalid Shader type"); break;
        }
    }

    void Texture2DAPI::Clean(Texture2DData* dataObject) {
        if (dataObject->m_ResourceView) {
            dataObject->m_ResourceView->Release();
            dataObject->m_ResourceView = 0;
        }

        if (dataObject->m_SamplerState) {
            dataObject->m_SamplerState->Release();
            dataObject->m_SamplerState = 0;
        }

        if (dataObject->m_Texture2D) {
            dataObject->m_Texture2D->Release();
            dataObject->m_Texture2D = 0;
        }

        free(dataObject->m_TexturePixels);
    }

    void* Texture2DAPI::GetResource(Texture2DData* dataObject) {
        return dataObject->m_ResourceView;
    }

    void Texture2DAPI::Load(Texture2DData* data) {
        D3D11_TEXTURE2D_DESC textureDescription;
        SecureZeroMemory(&textureDescription, sizeof(textureDescription));
        textureDescription.ArraySize = 1;
        textureDescription.SampleDesc.Count = 1;
        textureDescription.SampleDesc.Quality = 0;
        textureDescription.Usage = D3D11_USAGE_DEFAULT;
        textureDescription.CPUAccessFlags = 0;
        textureDescription.BindFlags = D3D11_BIND_SHADER_RESOURCE;
        textureDescription.MiscFlags = data->m_Settings.GenerateMipMaps ? D3D11_RESOURCE_MISC_GENERATE_MIPS : 0;

        // Set Format
        int width, height, channels;

        const char* path = data->m_Settings.TextureFilepath.c_str();
        data->m_HDR = stbi_is_hdr(path) ? true : false;
        data->m_TexturePixels = static_cast<void*>(stbi_load(path, &width, &height, &channels, 4));
        if (data->m_TexturePixels == nullptr) {
            ST_ENGINE_ERROR("Failed to load image at path : {0}", path);
            return;
        }

        textureDescription.Format =
            data->m_HDR ? DXGI_FORMAT_R32G32B32A32_FLOAT
                        : (data->m_Settings.sRGB ? DXGI_FORMAT_R8G8B8A8_UNORM_SRGB : DXGI_FORMAT_R8G8B8A8_UNORM);

        data->m_Width = width;
        data->m_Height = height;
        stbi_set_flip_vertically_on_load(false);
        // \Set Format

        textureDescription.Width = data->m_Width;
        textureDescription.Height = data->m_Height;
        textureDescription.MipLevels = data->m_HDR ? 1 : 0;

        if (data->m_Settings.GenerateMipMaps) textureDescription.BindFlags |= D3D11_BIND_RENDER_TARGET;

        ContextAPI::GetDevice(data->Context)->CreateTexture2D(&textureDescription, nullptr, &(data->m_Texture2D));

        UInt32 rowPitch = data->m_Width * 4 * (data->m_HDR ? sizeof(float) : sizeof(unsigned char));
        ContextAPI::GetNativeContext(data->Context)
            ->UpdateSubresource(data->m_Texture2D, 0, nullptr, data->m_TexturePixels, rowPitch, 0);

        D3D11_SHADER_RESOURCE_VIEW_DESC viewDescription;
        SecureZeroMemory(&viewDescription, sizeof(viewDescription));
        viewDescription.Format = textureDescription.Format;
        viewDescription.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
        viewDescription.Texture2D.MostDetailedMip = 0;
        viewDescription.Texture2D.MipLevels = data->m_Settings.GenerateMipMaps ? -1 : 1;

        ContextAPI::GetDevice(data->Context)
            ->CreateShaderResourceView(data->m_Texture2D, &viewDescription, &(data->m_ResourceView));

        if (data->m_Settings.GenerateMipMaps)
            ContextAPI::GetNativeContext(data->Context)->GenerateMips(data->m_ResourceView);

        {
            D3D11_SAMPLER_DESC samplerDesc;
            SecureZeroMemory(&samplerDesc, sizeof(samplerDesc));
            samplerDesc.Filter = D3D11_FILTER_ANISOTROPIC;
            samplerDesc.AddressU = s_WrapModeDescTable[data->m_Settings.WrapMode];
            samplerDesc.AddressV = s_WrapModeDescTable[data->m_Settings.WrapMode];
            samplerDesc.AddressW = s_WrapModeDescTable[data->m_Settings.WrapMode];
            samplerDesc.MaxAnisotropy = D3D11_REQ_MAXANISOTROPY;
            samplerDesc.MipLODBias = 0.0f;
            samplerDesc.MinLOD = 0.0f;
            samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

            ContextAPI::GetDevice(data->Context)->CreateSamplerState(&samplerDesc, &(data->m_SamplerState));
        }
    }

    void Texture2DAPI::Load(Texture2DData* data, UInt16 width, UInt16 height, UInt8 channels) {
        D3D11_TEXTURE2D_DESC textureDescription;
        SecureZeroMemory(&textureDescription, sizeof(textureDescription));
        textureDescription.ArraySize = 1;
        textureDescription.SampleDesc.Count = 1;
        textureDescription.SampleDesc.Quality = 0;
        textureDescription.Usage = D3D11_USAGE_DEFAULT;
        textureDescription.CPUAccessFlags = 0;
        textureDescription.BindFlags = D3D11_BIND_SHADER_RESOURCE;
        textureDescription.MiscFlags = data->m_Settings.GenerateMipMaps ? D3D11_RESOURCE_MISC_GENERATE_MIPS : 0;

        textureDescription.Format =
            data->m_HDR ? DXGI_FORMAT_R32G32B32A32_FLOAT
                        : (data->m_Settings.sRGB ? DXGI_FORMAT_R8G8B8A8_UNORM_SRGB : DXGI_FORMAT_R8G8B8A8_UNORM);

        data->m_Width = width;
        data->m_Height = height;
        stbi_set_flip_vertically_on_load(false);
        // \Set Format

        textureDescription.Width = data->m_Width;
        textureDescription.Height = data->m_Height;
        textureDescription.MipLevels = data->m_HDR ? 1 : 0;

        if (data->m_Settings.GenerateMipMaps) textureDescription.BindFlags |= D3D11_BIND_RENDER_TARGET;

        ContextAPI::GetDevice(data->Context)->CreateTexture2D(&textureDescription, nullptr, &(data->m_Texture2D));

        UInt32 rowPitch = data->m_Width * 4 * (data->m_HDR ? sizeof(float) : sizeof(unsigned char));

        ContextAPI::GetNativeContext(data->Context)
            ->UpdateSubresource(data->m_Texture2D, 0, nullptr, data->m_TexturePixels, rowPitch, 0);

        D3D11_SHADER_RESOURCE_VIEW_DESC viewDescription;
        SecureZeroMemory(&viewDescription, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
        viewDescription.Format = textureDescription.Format;
        viewDescription.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
        viewDescription.Texture2D.MostDetailedMip = 0;
        viewDescription.Texture2D.MipLevels = data->m_Settings.GenerateMipMaps ? -1 : 1;

        ContextAPI::GetDevice(data->Context)
            ->CreateShaderResourceView(data->m_Texture2D, &viewDescription, &(data->m_ResourceView));

        if (data->m_Settings.GenerateMipMaps)
            ContextAPI::GetNativeContext(data->Context)->GenerateMips(data->m_ResourceView);

        {
            D3D11_SAMPLER_DESC samplerDesc;
            SecureZeroMemory(&samplerDesc, sizeof(samplerDesc));
            samplerDesc.Filter = D3D11_FILTER_ANISOTROPIC;
            samplerDesc.AddressU = s_WrapModeDescTable[data->m_Settings.WrapMode];
            samplerDesc.AddressV = s_WrapModeDescTable[data->m_Settings.WrapMode];
            samplerDesc.AddressW = s_WrapModeDescTable[data->m_Settings.WrapMode];
            samplerDesc.MaxAnisotropy = D3D11_REQ_MAXANISOTROPY;
            samplerDesc.MipLODBias = 0.0f;
            samplerDesc.MinLOD = 0.0f;
            samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

            ContextAPI::GetDevice(data->Context)->CreateSamplerState(&samplerDesc, &(data->m_SamplerState));
        }
    }
}  // namespace Sentinel
#endif  // ST_RENDERER_DX11