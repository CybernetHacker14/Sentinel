#include "stpch.h"
#include "Platform/DirectX11/Graphics/Core/DX11Common.h"
#include "Platform/DirectX11/Graphics/Texture/DX11RenderTexture2DAPI.h"
#include "Platform/DirectX11/Graphics/Texture/DX11RenderTexture2DData.h"

#include "Platform/DirectX11/Graphics/Device/DX11ContextData.h"
#include "Platform/DirectX11/Graphics/Device/DX11ContextAPI.h"

namespace Sentinel {
    static const UInt32 s_MaxSize = 8192;  // 8K

    void DX11RenderTexture2DAPI::Create(DX11RenderTexture2DData* dataObject) {
        DX11ContextData* context = ContextAPI::Cast<DX11ContextData>(dataObject->Context);
        ID3D11Device* dxDevice = DX11ContextAPI::GetDevice(context);

        if (dataObject->m_ColorFormat != ColorFormat::NONE) {
            D3D11_TEXTURE2D_DESC texDescription;
            SecureZeroMemory(&texDescription, sizeof(texDescription));
            texDescription.Width = dataObject->m_Width;
            texDescription.Height = dataObject->m_Height;
            texDescription.MipLevels = 1;
            texDescription.ArraySize = 1;
            texDescription.Format = static_cast<DXGI_FORMAT>(dataObject->m_ColorFormat);
            texDescription.SampleDesc.Count = 1;
            texDescription.SampleDesc.Quality = 0;
            texDescription.Usage = D3D11_USAGE_DEFAULT;
            texDescription.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
            texDescription.CPUAccessFlags = 0;
            texDescription.MiscFlags = 0;

            dxDevice->CreateTexture2D(&texDescription, nullptr, &(dataObject->m_NativeColorTexture));

            // RTV
            D3D11_RENDER_TARGET_VIEW_DESC rTVDescription;
            SecureZeroMemory(&rTVDescription, sizeof(rTVDescription));
            rTVDescription.Format = static_cast<DXGI_FORMAT>(dataObject->m_ColorFormat);
            rTVDescription.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
            rTVDescription.Texture2D.MipSlice = 0;

            dxDevice->CreateRenderTargetView(
                dataObject->m_NativeColorTexture, &rTVDescription, &(dataObject->m_NativeColorRTV));

            // SRV
            {
                D3D11_SHADER_RESOURCE_VIEW_DESC sRVDescription;
                SecureZeroMemory(&sRVDescription, sizeof(sRVDescription));
                sRVDescription.Format = static_cast<DXGI_FORMAT>(dataObject->m_ColorFormat);
                sRVDescription.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
                sRVDescription.Texture2D.MipLevels = 1;
                sRVDescription.Texture2D.MostDetailedMip = 0;
                dxDevice->CreateShaderResourceView(
                    dataObject->m_NativeColorTexture, &sRVDescription, &(dataObject->m_NativeColorSRV));
            }
        }

        if (dataObject->m_DepthFormat != DepthFormat::NONE) {
            D3D11_TEXTURE2D_DESC texDescription;
            SecureZeroMemory(&texDescription, sizeof(texDescription));
            texDescription.Width = dataObject->m_Width;
            texDescription.Height = dataObject->m_Height;
            texDescription.MipLevels = 1;
            texDescription.ArraySize = 1;
            texDescription.Format = static_cast<DXGI_FORMAT>(dataObject->m_DepthFormat);
            texDescription.SampleDesc.Count = 1;
            texDescription.SampleDesc.Quality = 0;
            texDescription.Usage = D3D11_USAGE_DEFAULT;
            texDescription.CPUAccessFlags = 0;
            texDescription.MiscFlags = 0;
            texDescription.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;

            D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilDescription;
            SecureZeroMemory(&depthStencilDescription, sizeof(depthStencilDescription));
            depthStencilDescription.Format = static_cast<DXGI_FORMAT>(dataObject->m_DepthFormat);
            depthStencilDescription.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
            depthStencilDescription.Texture2D.MipSlice = 0;

            dxDevice->CreateDepthStencilView(
                dataObject->m_NativeDepthTexture, &depthStencilDescription, &(dataObject->m_NativeDepthDSV));
        }
    }

    void DX11RenderTexture2DAPI::Clear(RenderTexture2DData* dataObject, const glm::vec4& clearColor) {
        DX11ContextData* context = ContextAPI::Cast<DX11ContextData>(dataObject->Context);
        ID3D11DeviceContext* dxContext = DX11ContextAPI::GetNativeContext(context);

        DX11RenderTexture2DData* dxDataObject = RenderTexture2DAPI::Cast<DX11RenderTexture2DData>(dataObject);

        ShaderType colorType = RenderTexture2DAPI::GetBindColorType(dataObject);
        ShaderType depthType = RenderTexture2DAPI::GetBindDepthType(dataObject);

        if (colorType != ShaderType::NONE) {
            dxContext->ClearRenderTargetView(
                DX11RenderTexture2DAPI::GetNativeColorRTV(dxDataObject), (Float*)&clearColor);
        }

        if (depthType != ShaderType::NONE) {
            dxContext->ClearDepthStencilView(
                DX11RenderTexture2DAPI::GetNativeDepthDSV(dxDataObject),
                D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,
                1.0f,
                0);
        }
    }

    void DX11RenderTexture2DAPI::Clean(RenderTexture2DData* dataObject) {
        DX11RenderTexture2DData* dxDataObject = RenderTexture2DAPI::Cast<DX11RenderTexture2DData>(dataObject);

        ShaderType colorType = RenderTexture2DAPI::GetBindColorType(dataObject);
        ShaderType depthType = RenderTexture2DAPI::GetBindDepthType(dataObject);

        if (colorType != ShaderType::NONE) {
            dxDataObject->m_NativeColorTexture->Release();
            dxDataObject->m_NativeColorRTV->Release();
            dxDataObject->m_NativeColorSRV->Release();
            dxDataObject->m_NativeColorUAV->Release();
        }

        if (colorType != ShaderType::NONE) {
            dxDataObject->m_NativeDepthTexture->Release();
            dxDataObject->m_NativeDepthDSV->Release();
            dxDataObject->m_NativeDepthSRV->Release();
        }
    }

    void DX11RenderTexture2DAPI::Resize(RenderTexture2DData* dataObject, UInt32 width, UInt32 height) {
        if (width == 0 || height == 0 || width > s_MaxSize || height > s_MaxSize) return;

        Unbind(dataObject);
        Clean(dataObject);
        RenderTexture2DAPI::SetWidth(dataObject, width);
        RenderTexture2DAPI::SetHeight(dataObject, height);
        Create(RenderTexture2DAPI::Cast<DX11RenderTexture2DData>(dataObject));
    }

    void DX11RenderTexture2DAPI::BindColor(RenderTexture2DData* dataObject, UInt32 slot, const ShaderType shaderType) {
        DX11ContextData* context = ContextAPI::Cast<DX11ContextData>(dataObject->Context);
        ID3D11DeviceContext* dxContext = DX11ContextAPI::GetNativeContext(context);

        DX11RenderTexture2DData* dxDataObject = RenderTexture2DAPI::Cast<DX11RenderTexture2DData>(dataObject);
        ID3D11ShaderResourceView* srv = DX11RenderTexture2DAPI::GetNativeColorSRV(dxDataObject);

        switch (shaderType) {
            case ShaderType::VERTEX:
                dxContext->VSSetShaderResources(slot, 1, &srv);
                RenderTexture2DAPI::SetBindColorSlot(dataObject, slot);
                RenderTexture2DAPI::SetBindColorType(dataObject, shaderType);
                break;
            case ShaderType::PIXEL:
                dxContext->PSSetShaderResources(slot, 1, &srv);
                RenderTexture2DAPI::SetBindColorSlot(dataObject, slot);
                RenderTexture2DAPI::SetBindColorType(dataObject, shaderType);
                break;
            case ShaderType::COMPUTE:
                dxContext->CSSetShaderResources(slot, 1, &srv);
                RenderTexture2DAPI::SetBindColorSlot(dataObject, slot);
                RenderTexture2DAPI::SetBindColorType(dataObject, shaderType);
                break;
            case ShaderType::NONE: ST_ENGINE_ASSERT(false, "Invalid shader type"); break;
        }
    }

    void DX11RenderTexture2DAPI::BindDepth(RenderTexture2DData* dataObject, UInt32 slot, const ShaderType shaderType) {
        DX11ContextData* context = ContextAPI::Cast<DX11ContextData>(dataObject->Context);
        ID3D11DeviceContext* dxContext = DX11ContextAPI::GetNativeContext(context);

        DX11RenderTexture2DData* dxDataObject = RenderTexture2DAPI::Cast<DX11RenderTexture2DData>(dataObject);
        ID3D11ShaderResourceView* srv = DX11RenderTexture2DAPI::GetNativeDepthSRV(dxDataObject);

        switch (shaderType) {
            case ShaderType::VERTEX:
                dxContext->VSSetShaderResources(slot, 1, &srv);
                RenderTexture2DAPI::SetBindDepthSlot(dataObject, slot);
                RenderTexture2DAPI::SetBindDepthType(dataObject, shaderType);
                break;
            case ShaderType::PIXEL:
                dxContext->PSSetShaderResources(slot, 1, &srv);
                RenderTexture2DAPI::SetBindDepthSlot(dataObject, slot);
                RenderTexture2DAPI::SetBindDepthType(dataObject, shaderType);
                break;
            case ShaderType::COMPUTE:
                dxContext->CSSetShaderResources(slot, 1, &srv);
                RenderTexture2DAPI::SetBindDepthSlot(dataObject, slot);
                RenderTexture2DAPI::SetBindDepthType(dataObject, shaderType);
                break;
            case ShaderType::NONE: ST_ENGINE_ASSERT(false, "Invalid shader type"); break;
        }
    }

    void DX11RenderTexture2DAPI::Unbind(RenderTexture2DData* dataObject) {
        DX11ContextData* context = ContextAPI::Cast<DX11ContextData>(dataObject->Context);
        ID3D11DeviceContext* dxContext = DX11ContextAPI::GetNativeContext(context);

        DX11RenderTexture2DData* dxDataObject = RenderTexture2DAPI::Cast<DX11RenderTexture2DData>(dataObject);
        UInt32 colorSlot = RenderTexture2DAPI::GetBindColorSlot(dataObject);
        UInt32 depthSlot = RenderTexture2DAPI::GetBindDepthSlot(dataObject);
        ShaderType colorType = RenderTexture2DAPI::GetBindColorType(dataObject);
        ShaderType depthType = RenderTexture2DAPI::GetBindDepthType(dataObject);

        if (colorType != ShaderType::NONE) {
            switch (colorType) {
                case ShaderType::VERTEX:
                    dxContext->VSSetShaderResources(colorSlot, 1, nullptr);
                    RenderTexture2DAPI::SetBindDepthType(dataObject, ShaderType::NONE);
                    break;
                case ShaderType::PIXEL:
                    dxContext->PSSetShaderResources(colorSlot, 1, nullptr);
                    RenderTexture2DAPI::SetBindDepthType(dataObject, ShaderType::NONE);
                    break;
                case ShaderType::COMPUTE:
                    dxContext->CSSetShaderResources(colorSlot, 1, nullptr);
                    RenderTexture2DAPI::SetBindDepthType(dataObject, ShaderType::NONE);
                    break;
                case ShaderType::NONE: ST_ENGINE_ASSERT(false, "Invalid shader type"); break;
            }
        }

        if (depthType != ShaderType::NONE) {
            switch (depthType) {
                case ShaderType::VERTEX:
                    dxContext->VSSetShaderResources(depthSlot, 1, nullptr);
                    RenderTexture2DAPI::SetBindDepthType(dataObject, ShaderType::NONE);
                    break;
                case ShaderType::PIXEL:
                    dxContext->PSSetShaderResources(depthSlot, 1, nullptr);
                    RenderTexture2DAPI::SetBindDepthType(dataObject, ShaderType::NONE);
                    break;
                case ShaderType::COMPUTE:
                    dxContext->CSSetShaderResources(depthSlot, 1, nullptr);
                    RenderTexture2DAPI::SetBindDepthType(dataObject, ShaderType::NONE);
                    break;
                case ShaderType::NONE: ST_ENGINE_ASSERT(false, "Invalid shader type"); break;
            }
        }
    }
}  // namespace Sentinel