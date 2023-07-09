#include "stpch.h"

#ifdef ST_RENDERER_DX11

    #include "Sentinel/Graphics/Texture/RenderTexture2DAPI.h"
    #include "Sentinel/Graphics/Device/ContextAPI.h"
    #include "Sentinel/Graphics/Device/SwapchainAPI.h"

    #include "Platform/DirectX11/Graphics/Core/DX11Common.h"

    #include <glm/glm.hpp>

namespace Sentinel {
    static const UInt32 s_MaxSize = 8192;  // 8K

    RenderTexture2DData* RenderTexture2DAPI::CreateRenderTexture2DData(
        PoolAllocator<RenderTexture2DData>& allocator,
        ContextData* context,
        const UInt16 width,
        const UInt16 height,
        const ColorFormat format) {
        RenderTexture2DData* texObject = allocator.New();
        texObject->Context = context;
        texObject->m_Format = format;
        texObject->m_Width = width;
        texObject->m_Height = height;
        texObject->m_BindType = ShaderType::NONE;
        texObject->m_SwapchainTarget = false;
        Create(texObject);
        return texObject;
    }

    RenderTexture2DData* RenderTexture2DAPI::CreateRenderTexture2DData(
        PoolAllocator<RenderTexture2DData>& allocator, ContextData* context, SwapchainData* swapchain) {
        RenderTexture2DData* texObject = allocator.New();
        texObject->Context = context;
        texObject->m_Format = ColorFormat::RGBA32F;
        texObject->m_BindType = ShaderType::NONE;
        texObject->m_SwapchainTarget = true;
        texObject->TargetSwapchain = swapchain;
        Create(texObject, swapchain);
        return texObject;
    }

    void RenderTexture2DAPI::Clear(RenderTexture2DData* dataObject, const glm::vec4& clearColor) {
        ID3D11DeviceContext* dxContext = ContextAPI::GetNativeContext(dataObject->Context);

        if (!dataObject->m_NativeRTV) { return; }

        if (dataObject->m_BindType != ShaderType::NONE)
            dxContext->ClearRenderTargetView(dataObject->m_NativeRTV, (Float*)&clearColor);
    }

    void RenderTexture2DAPI::Clean(RenderTexture2DData* dataObject) {
        if (dataObject->m_NativeRTV) {
            dataObject->m_NativeRTV->Release();
            dataObject->m_NativeRTV = 0;
        }

        if (!dataObject->m_SwapchainTarget && dataObject->m_NativeSRV) {
            dataObject->m_NativeSRV->Release();
            dataObject->m_NativeSRV = 0;
        }

        if (dataObject->m_NativeTex) {
            dataObject->m_NativeTex->Release();
            dataObject->m_NativeTex = 0;
        }
    }

    void RenderTexture2DAPI::Bind(RenderTexture2DData* dataObject, UInt8 slot, const ShaderType shaderType) {
        ID3D11DeviceContext* dxContext = ContextAPI::GetNativeContext(dataObject->Context);

        switch (shaderType) {
            case ShaderType::VERTEX:
                dxContext->VSSetShaderResources(slot, 1, &(dataObject->m_NativeSRV));
                dataObject->m_BindSlot = slot;
                dataObject->m_BindType = shaderType;
                break;
            case ShaderType::PIXEL:
                dxContext->PSSetShaderResources(slot, 1, &(dataObject->m_NativeSRV));
                dataObject->m_BindSlot = slot;
                dataObject->m_BindType = shaderType;
                break;
            case ShaderType::COMPUTE:
                dxContext->CSSetShaderResources(slot, 1, &(dataObject->m_NativeSRV));
                dataObject->m_BindSlot = slot;
                dataObject->m_BindType = shaderType;
                break;
            case ShaderType::NONE: ST_ENGINE_ASSERT(false, "Invalid shader type"); break;
        }
    }

    void RenderTexture2DAPI::Unbind(RenderTexture2DData* dataObject) {
        ID3D11DeviceContext* dxContext = ContextAPI::GetNativeContext(dataObject->Context);
        ID3D11ShaderResourceView* nullSRV = {nullptr};

        if (dataObject->m_BindType == ShaderType::NONE) return;

        switch (dataObject->m_BindType) {
            case ShaderType::VERTEX:
                dxContext->VSSetShaderResources(dataObject->m_BindSlot, 1, &nullSRV);
                dataObject->m_BindType = ShaderType::NONE;
                break;
            case ShaderType::PIXEL:
                dxContext->PSSetShaderResources(dataObject->m_BindSlot, 1, &nullSRV);
                dataObject->m_BindType = ShaderType::NONE;
                break;
            case ShaderType::COMPUTE:
                dxContext->CSSetShaderResources(dataObject->m_BindSlot, 1, &nullSRV);
                dataObject->m_BindType = ShaderType::NONE;
                break;
        }
    }

    void RenderTexture2DAPI::Resize(RenderTexture2DData* dataObject, UInt16 width, UInt16 height) {
        if (width == 0 || height == 0 || width > s_MaxSize || height > s_MaxSize) return;

        dataObject->m_Width = width;
        dataObject->m_Height = height;

        if (dataObject->m_SwapchainTarget)
            Create(dataObject, dataObject->TargetSwapchain);
        else
            Create(dataObject);
    }

    void RenderTexture2DAPI::Create(RenderTexture2DData* dataObject) {
        ID3D11Device* dxDevice = ContextAPI::GetDevice(dataObject->Context);

        if (dataObject->m_Format != ColorFormat::NONE) {
            D3D11_TEXTURE2D_DESC texDescription;
            SecureZeroMemory(&texDescription, sizeof(texDescription));
            texDescription.Width = dataObject->m_Width;
            texDescription.Height = dataObject->m_Height;
            texDescription.MipLevels = 1;
            texDescription.ArraySize = 1;
            texDescription.Format = static_cast<DXGI_FORMAT>(dataObject->m_Format);
            texDescription.SampleDesc.Count = 1;
            texDescription.SampleDesc.Quality = 0;
            texDescription.Usage = D3D11_USAGE_DEFAULT;
            texDescription.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
            texDescription.CPUAccessFlags = 0;
            texDescription.MiscFlags = 0;

            dxDevice->CreateTexture2D(&texDescription, nullptr, &(dataObject->m_NativeTex));

            // RTV
            D3D11_RENDER_TARGET_VIEW_DESC rTVDescription;
            SecureZeroMemory(&rTVDescription, sizeof(rTVDescription));
            rTVDescription.Format = static_cast<DXGI_FORMAT>(dataObject->m_Format);
            rTVDescription.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
            rTVDescription.Texture2D.MipSlice = 0;

            dxDevice->CreateRenderTargetView(dataObject->m_NativeTex, &rTVDescription, &(dataObject->m_NativeRTV));

            // SRV
            {
                D3D11_SHADER_RESOURCE_VIEW_DESC sRVDescription;
                SecureZeroMemory(&sRVDescription, sizeof(sRVDescription));
                sRVDescription.Format = static_cast<DXGI_FORMAT>(dataObject->m_Format);
                sRVDescription.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
                sRVDescription.Texture2D.MipLevels = 1;
                sRVDescription.Texture2D.MostDetailedMip = 0;
                dxDevice->CreateShaderResourceView(
                    dataObject->m_NativeTex, &sRVDescription, &(dataObject->m_NativeSRV));
            }
        }
    }

    void RenderTexture2DAPI::Create(RenderTexture2DData* dataObject, SwapchainData* swapchain) {
        ID3D11Device* dxDevice = ContextAPI::GetDevice(dataObject->Context);
        IDXGISwapChain* nativeSC = SwapchainAPI::GetNativeSwapchain(swapchain);

        nativeSC->GetBuffer(0, __uuidof(ID3D11Resource), (LPVOID*)&(dataObject->m_NativeTex));
        dxDevice->CreateRenderTargetView(dataObject->m_NativeTex, nullptr, &(dataObject->m_NativeRTV));
    }

}  // namespace Sentinel
#endif  // ST_RENDERER_DX11