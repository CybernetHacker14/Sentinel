#include "stpch.h"
#include "Platform/DirectX11/Graphics/Core/DX11Common.h"
#include "Platform/DirectX11/Graphics/Texture/DX11RenderTexture2DAPI.h"

#include "Platform/DirectX11/Graphics/Device/DX11ContextAPI.h"

namespace Sentinel {
    static const UInt32 s_MaxSize = 8192;  // 8K

    DX11RenderTexture2DAPI::_init DX11RenderTexture2DAPI::_initializer;

    void DX11RenderTexture2DAPI::Create(DX11RenderTexture2DData* dataObject) {
        DX11ContextData* context = ContextAPI::Cast<DX11ContextData>(dataObject->Context);
        ID3D11Device* dxDevice = DX11ContextAPI::GetDevice(context);

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

            dxDevice->CreateTexture2D(&texDescription, nullptr, &(dataObject->m_NativeTexture));

            // RTV
            D3D11_RENDER_TARGET_VIEW_DESC rTVDescription;
            SecureZeroMemory(&rTVDescription, sizeof(rTVDescription));
            rTVDescription.Format = static_cast<DXGI_FORMAT>(dataObject->m_Format);
            rTVDescription.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
            rTVDescription.Texture2D.MipSlice = 0;

            dxDevice->CreateRenderTargetView(dataObject->m_NativeTexture, &rTVDescription, &(dataObject->m_NativeRTV));

            // SRV
            {
                D3D11_SHADER_RESOURCE_VIEW_DESC sRVDescription;
                SecureZeroMemory(&sRVDescription, sizeof(sRVDescription));
                sRVDescription.Format = static_cast<DXGI_FORMAT>(dataObject->m_Format);
                sRVDescription.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
                sRVDescription.Texture2D.MipLevels = 1;
                sRVDescription.Texture2D.MostDetailedMip = 0;
                dxDevice->CreateShaderResourceView(
                    dataObject->m_NativeTexture, &sRVDescription, &(dataObject->m_NativeSRV));
            }
        }
    }

    void DX11RenderTexture2DAPI::Create(DX11RenderTexture2DData* dataObject, DX11SwapchainData* swapchain) {
        DX11ContextData* context = ContextAPI::Cast<DX11ContextData>(dataObject->Context);
        ID3D11Device* dxDevice = DX11ContextAPI::GetDevice(context);
        IDXGISwapChain* nativeSC = DX11SwapchainAPI::GetNativeSwapchain(swapchain);

        nativeSC->GetBuffer(0, __uuidof(ID3D11Resource), (LPVOID*)&(dataObject->m_NativeTexture));
        dxDevice->CreateRenderTargetView(dataObject->m_NativeTexture, nullptr, &(dataObject->m_NativeRTV));

        dataObject->m_TargetSwapchain = swapchain;
    }

    void DX11RenderTexture2DAPI::Clear(RenderTexture2DData* dataObject, const glm::vec4& clearColor) {
        DX11ContextData* context = ContextAPI::Cast<DX11ContextData>(dataObject->Context);
        ID3D11DeviceContext* dxContext = DX11ContextAPI::GetNativeContext(context);

        DX11RenderTexture2DData* dxDataObject = RenderTexture2DAPI::Cast<DX11RenderTexture2DData>(dataObject);

        if (dxDataObject->m_BindType != ShaderType::NONE) {
            dxContext->ClearRenderTargetView(dxDataObject->m_NativeRTV, (Float*)&clearColor);
        }
    }

    void DX11RenderTexture2DAPI::Clean(RenderTexture2DData* dataObject) {
        DX11RenderTexture2DData* dxDataObject = RenderTexture2DAPI::Cast<DX11RenderTexture2DData>(dataObject);

        if (dxDataObject->m_NativeRTV) dxDataObject->m_NativeRTV->Release();
        if (dxDataObject->m_NativeSRV) dxDataObject->m_NativeSRV->Release();
        if (dxDataObject->m_NativeUAV) dxDataObject->m_NativeUAV->Release();
        if (dxDataObject->m_NativeTexture) dxDataObject->m_NativeTexture->Release();
    }

    void DX11RenderTexture2DAPI::Bind(RenderTexture2DData* dataObject, UInt32 slot, const ShaderType shaderType) {
        DX11ContextData* context = ContextAPI::Cast<DX11ContextData>(dataObject->Context);
        ID3D11DeviceContext* dxContext = DX11ContextAPI::GetNativeContext(context);

        DX11RenderTexture2DData* dxDataObject = RenderTexture2DAPI::Cast<DX11RenderTexture2DData>(dataObject);
        ID3D11ShaderResourceView* srv = dxDataObject->m_NativeSRV;

        switch (shaderType) {
            case ShaderType::VERTEX:
                dxContext->VSSetShaderResources(slot, 1, &srv);
                dxDataObject->m_BindSlot = slot;
                dxDataObject->m_BindType = shaderType;
                break;
            case ShaderType::PIXEL:
                dxContext->PSSetShaderResources(slot, 1, &srv);
                dxDataObject->m_BindSlot = slot;
                dxDataObject->m_BindType = shaderType;
                break;
            case ShaderType::COMPUTE:
                dxContext->CSSetShaderResources(slot, 1, &srv);
                dxDataObject->m_BindSlot = slot;
                dxDataObject->m_BindType = shaderType;
                break;
            case ShaderType::NONE: ST_ENGINE_ASSERT(false, "Invalid shader type"); break;
        }
    }

    void DX11RenderTexture2DAPI::Unbind(RenderTexture2DData* dataObject) {
        DX11ContextData* context = ContextAPI::Cast<DX11ContextData>(dataObject->Context);
        ID3D11DeviceContext* dxContext = DX11ContextAPI::GetNativeContext(context);

        DX11RenderTexture2DData* dxDataObject = RenderTexture2DAPI::Cast<DX11RenderTexture2DData>(dataObject);

        ID3D11ShaderResourceView* nullSRV = {nullptr};

        if (dxDataObject->m_BindType != ShaderType::NONE) {
            switch (dxDataObject->m_BindType) {
                case ShaderType::VERTEX:
                    dxContext->VSSetShaderResources(dxDataObject->m_BindSlot, 1, &nullSRV);
                    dxDataObject->m_BindType = ShaderType::NONE;
                    break;
                case ShaderType::PIXEL:
                    dxContext->PSSetShaderResources(dxDataObject->m_BindSlot, 1, &nullSRV);
                    dxDataObject->m_BindType = ShaderType::NONE;
                    break;
                case ShaderType::COMPUTE:
                    dxContext->CSSetShaderResources(dxDataObject->m_BindSlot, 1, &nullSRV);
                    dxDataObject->m_BindType = ShaderType::NONE;
                    break;
                case ShaderType::NONE: ST_ENGINE_ASSERT(false, "Invalid shader type"); break;
            }
        }
    }
    void DX11RenderTexture2DAPI::Resize(RenderTexture2DData* dataObject, UInt16 width, UInt16 height) {
        if (width == 0 || height == 0 || width > s_MaxSize || height > s_MaxSize) return;

        DX11RenderTexture2DData* dxDataObject = RenderTexture2DAPI::Cast<DX11RenderTexture2DData>(dataObject);

        dxDataObject->m_Width = width;
        dxDataObject->m_Height = height;

        if (dxDataObject->m_SwapchainTarget)
            Create(dxDataObject, dxDataObject->m_TargetSwapchain);
        else {};
    }
}  // namespace Sentinel