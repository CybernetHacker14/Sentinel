#include "stpch.h"
#include "Platform/DirectX11/Graphics/Core/DX11Common.h"
#include "Platform/DirectX11/Graphics/Texture/DX11DepthTexture2DAPI.h"

#include "Platform/DirectX11/Graphics/Device/DX11ContextAPI.h"

namespace Sentinel {
    static const UInt32 s_MaxSize = 8192;  // 8K

    DX11DepthTexture2DAPI::_init DX11DepthTexture2DAPI::_initializer;

    void DX11DepthTexture2DAPI::Create(DX11DepthTexture2DData* dataObject) {
        DX11ContextData* context = ContextAPI::Cast<DX11ContextData>(dataObject->Context);
        ID3D11Device* dxDevice = DX11ContextAPI::GetDevice(context);

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

            dxDevice->CreateTexture2D(&texDescription, nullptr, &(dataObject->m_NativeTexture));

            // DSV
            D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilDescription;
            SecureZeroMemory(&depthStencilDescription, sizeof(depthStencilDescription));
            depthStencilDescription.Format = static_cast<DXGI_FORMAT>(dataObject->m_Format);
            depthStencilDescription.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
            depthStencilDescription.Texture2D.MipSlice = 0;

            dxDevice->CreateDepthStencilView(
                dataObject->m_NativeTexture, &depthStencilDescription, &(dataObject->m_NativeDSV));

            // SRV
            if (!dataObject->m_SwapchainTarget) {
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

    void DX11DepthTexture2DAPI::Clear(DepthTexture2DData* dataObject) {
        DX11DepthTexture2DData* depthTexture = DepthTexture2DAPI::Cast<DX11DepthTexture2DData>(dataObject);

        DX11ContextData* context = ContextAPI::Cast<DX11ContextData>(dataObject->Context);
        ID3D11DeviceContext* dxContext = DX11ContextAPI::GetNativeContext(context);

        if (depthTexture->m_BindType != ShaderType::NONE) {
            dxContext->ClearDepthStencilView(
                depthTexture->m_NativeDSV, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
        }
    }

    void DX11DepthTexture2DAPI::Clean(DepthTexture2DData* dataObject) {
        DX11DepthTexture2DData* dxDataObject = DepthTexture2DAPI::Cast<DX11DepthTexture2DData>(dataObject);

        if (dxDataObject->m_BindType != ShaderType::NONE) {
            dxDataObject->m_NativeTexture->Release();
            dxDataObject->m_NativeDSV->Release();
            dxDataObject->m_NativeSRV->Release();
        }
    }

    void DX11DepthTexture2DAPI::Resize(DepthTexture2DData* dataObject, UInt32 width, UInt32 height) {
        if (width == 0 || height == 0 || width > s_MaxSize || height > s_MaxSize) return;

        DX11DepthTexture2DData* dxDataObject = DepthTexture2DAPI::Cast<DX11DepthTexture2DData>(dataObject);

        Unbind(dataObject);
        Clean(dataObject);
        dxDataObject->m_Width = width;
        dxDataObject->m_Height = height;
        Create(dxDataObject);
    }

    void DX11DepthTexture2DAPI::Bind(DepthTexture2DData* dataObject, UInt32 slot, const ShaderType shaderType) {
        DX11ContextData* context = ContextAPI::Cast<DX11ContextData>(dataObject->Context);
        ID3D11DeviceContext* dxContext = DX11ContextAPI::GetNativeContext(context);

        DX11DepthTexture2DData* dxDataObject = DepthTexture2DAPI::Cast<DX11DepthTexture2DData>(dataObject);
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

    void DX11DepthTexture2DAPI::Unbind(DepthTexture2DData* dataObject) {
        DX11ContextData* context = ContextAPI::Cast<DX11ContextData>(dataObject->Context);
        ID3D11DeviceContext* dxContext = DX11ContextAPI::GetNativeContext(context);

        DX11DepthTexture2DData* dxDataObject = DepthTexture2DAPI::Cast<DX11DepthTexture2DData>(dataObject);

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
}  // namespace Sentinel