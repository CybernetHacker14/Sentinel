#include "stpch.h"

#ifdef ST_RENDERER_DX11

    #include "Sentinel/Graphics/Texture/DepthTexture2DAPI.h"
    #include "Sentinel/Graphics/Device/ContextAPI.h"
    #include "Sentinel/Graphics/Device/SwapchainAPI.h"

    #include "Platform/DirectX11/Graphics/Core/DX11Common.h"

namespace Sentinel {
    static const UInt32 s_MaxSize = 8192;  // 8K

    DepthTexture2DData* Sentinel::DepthTexture2DAPI::CreateDepthTexture2DData(
        PoolAllocator<DepthTexture2DData>& allocator,
        ContextData* context,
        const UInt16 width,
        const UInt16 height,
        const DepthFormat format,
        const Bool attachToSwapchain) {
        DepthTexture2DData* texObject = allocator.New();
        texObject->Context = context;
        texObject->m_Format = format;
        texObject->m_Width = width;
        texObject->m_Height = height;
        texObject->m_BindType = ShaderType::NONE;
        texObject->m_SwapchainTarget = attachToSwapchain;
        Create(texObject);
        return texObject;
    }

    void DepthTexture2DAPI::Clear(DepthTexture2DData* dataObject) {
        ID3D11DeviceContext* dxContext = ContextAPI::GetNativeContext(dataObject->Context);

        if (dataObject->m_BindType != ShaderType::NONE) {
            dxContext->ClearDepthStencilView(dataObject->m_NativeDSV, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
        }
    }

    void DepthTexture2DAPI::Clean(DepthTexture2DData* dataObject) {
        if (dataObject->m_NativeDSV) {
            dataObject->m_NativeDSV->Release();
            dataObject->m_NativeDSV = 0;
        }

        if (dataObject->m_NativeSRV) {
            dataObject->m_NativeSRV->Release();
            dataObject->m_NativeSRV = 0;
        }

        if (dataObject->m_NativeTex) {
            dataObject->m_NativeTex->Release();
            dataObject->m_NativeTex = 0;
        }
    }

    void DepthTexture2DAPI::Bind(DepthTexture2DData* dataObject, UInt8 slot, const ShaderType shaderType) {
        ID3D11DeviceContext* dxContext = ContextAPI::GetNativeContext(dataObject->Context);
        ID3D11ShaderResourceView* srv = dataObject->m_NativeSRV;

        if (shaderType == ShaderType::NONE) return;

        switch (shaderType) {
            case ShaderType::VERTEX:
                dxContext->VSSetShaderResources(slot, 1, &srv);
                dataObject->m_BindSlot = slot;
                dataObject->m_BindType = shaderType;
                break;
            case ShaderType::PIXEL:
                dxContext->PSSetShaderResources(slot, 1, &srv);
                dataObject->m_BindSlot = slot;
                dataObject->m_BindType = shaderType;
                break;
            case ShaderType::COMPUTE:
                dxContext->CSSetShaderResources(slot, 1, &srv);
                dataObject->m_BindSlot = slot;
                dataObject->m_BindType = shaderType;
                break;
        }
    }

    void DepthTexture2DAPI::Unbind(DepthTexture2DData* dataObject) {
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

    void DepthTexture2DAPI::Resize(DepthTexture2DData* dataObject, UInt16 width, UInt16 height) {
        if (width == 0 || height == 0 || width > s_MaxSize || height > s_MaxSize) return;

        dataObject->m_Width = width;
        dataObject->m_Height = height;
        Create(dataObject);
    }

    void DepthTexture2DAPI::Create(DepthTexture2DData* dataObject) {
        ID3D11Device* dxDevice = ContextAPI::GetDevice(dataObject->Context);

        if (dataObject->m_Format != DepthFormat::NONE) {
            D3D11_TEXTURE2D_DESC texDescription;
            SecureZeroMemory(&texDescription, sizeof(texDescription));
            texDescription.Width = dataObject->m_Width;
            texDescription.Height = dataObject->m_Height;
            texDescription.MipLevels = 1;
            texDescription.ArraySize = 1;
            texDescription.Format = static_cast<DXGI_FORMAT>(dataObject->m_Format);
            texDescription.SampleDesc.Count = 4;
            texDescription.SampleDesc.Quality = 0;
            texDescription.Usage = D3D11_USAGE_DEFAULT;
            texDescription.CPUAccessFlags = 0;
            texDescription.MiscFlags = 0;
            texDescription.BindFlags = D3D11_BIND_DEPTH_STENCIL;

            dxDevice->CreateTexture2D(&texDescription, nullptr, &(dataObject->m_NativeTex));

            // DSV
            D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilDescription;
            SecureZeroMemory(&depthStencilDescription, sizeof(depthStencilDescription));
            depthStencilDescription.Format = static_cast<DXGI_FORMAT>(dataObject->m_Format);
            depthStencilDescription.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;
            depthStencilDescription.Texture2D.MipSlice = 0;

            dxDevice->CreateDepthStencilView(
                dataObject->m_NativeTex, &depthStencilDescription, &(dataObject->m_NativeDSV));

            // SRV
            if (!dataObject->m_SwapchainTarget) {
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

}  // namespace Sentinel
#endif  // ST_RENDERER_DX11