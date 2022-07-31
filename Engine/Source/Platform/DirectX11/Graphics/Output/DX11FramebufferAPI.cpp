#include "stpch.h"
#include "Platform/DirectX11/Graphics/Core/DX11Common.h"
#include "Platform/DirectX11/Graphics/Output/DX11FramebufferAPI.h"

#include "Platform/DirectX11/Graphics/Device/DX11ContextAPI.h"
#include "Platform/DirectX11/Graphics/Texture/DX11RenderTexture2DAPI.h"
#include "Platform/DirectX11/Graphics/Texture/DX11DepthTexture2DAPI.h"
#include "Platform/DirectX11/Graphics/Device/DX11SwapchainAPI.h"

namespace Sentinel {
    void DX11FramebufferAPI::Create(DX11FramebufferData* dataObject) {
        SecureZeroMemory(&(dataObject->m_Viewport), sizeof(dataObject->m_Viewport));
    }

    void DX11FramebufferAPI::Invalidate(FramebufferData* dataObject, SharedRef<GraphicsMemoryManager> memoryHandle) {
        DX11FramebufferData* framebuffer = FramebufferAPI::Cast<DX11FramebufferData>(dataObject);
        framebuffer->m_Viewport.TopLeftX = 0.0f;
        framebuffer->m_Viewport.TopLeftY = 0.0f;
        framebuffer->m_Viewport.Width = static_cast<Float>(framebuffer->m_Width);
        framebuffer->m_Viewport.Height = static_cast<Float>(framebuffer->m_Height);
        framebuffer->m_Viewport.MinDepth = 0.0f;
        framebuffer->m_Viewport.MaxDepth = 1.0f;

        for (UInt16 i = 0; i < framebuffer->m_ColorFormats.size(); i++) {
            framebuffer->m_RTAttachments[i] = RenderTexture2DAPI::CreateRenderTexture2DData(
                memoryHandle,
                framebuffer->Context,
                framebuffer->m_Width,
                framebuffer->m_Height,
                framebuffer->m_ColorFormats[i]);
        }

        framebuffer->m_DTAttachment = DepthTexture2DAPI::CreateDepthTexture2DData(
            memoryHandle,
            framebuffer->Context,
            framebuffer->m_Width,
            framebuffer->m_Height,
            framebuffer->m_DepthFormat);
    }

    void DX11FramebufferAPI::Bind(FramebufferData* dataObject) {
        DX11FramebufferData* framebuffer = FramebufferAPI::Cast<DX11FramebufferData>(dataObject);
        DX11ContextData* dxContext = ContextAPI::Cast<DX11ContextData>(dataObject->Context);
        ID3D11DeviceContext* nativeContext = DX11ContextAPI::GetNativeContext(dxContext);

        nativeContext->RSSetViewports(1, &(framebuffer->m_Viewport));
        Microsoft::WRL::ComPtr<ID3D11RenderTargetView> pRenderViews[15];

        DX11RenderTexture2DData* dxRenderTexture = nullptr;

        for (UInt16 i = 0; i < framebuffer->m_ColorFormats.size(); i++) {
            dxRenderTexture = RenderTexture2DAPI::Cast<DX11RenderTexture2DData>(framebuffer->m_RTAttachments[i]);
            pRenderViews[i] = dxRenderTexture->m_NativeRTV;
        }

        DX11DepthTexture2DData* dxDepthTexture =
            DepthTexture2DAPI::Cast<DX11DepthTexture2DData>(framebuffer->m_DTAttachment);

        nativeContext->OMSetRenderTargets(
            framebuffer->m_ColorFormats.size(),
            pRenderViews[0].GetAddressOf(),
            framebuffer->m_DepthFormat == DepthFormat::NONE ? NULL : dxDepthTexture->m_NativeDSV);
    }

    void DX11FramebufferAPI::Unbind(FramebufferData* dataObject) {
        ID3D11RenderTargetView* null = nullptr;
        DX11ContextData* dxContext = ContextAPI::Cast<DX11ContextData>(dataObject->Context);
        DX11ContextAPI::GetNativeContext(dxContext)->OMSetRenderTargets(1, &null, NULL);
    }

    void DX11FramebufferAPI::Clear(FramebufferData* dataObject, const glm::vec4& clearColor) {
        DX11FramebufferData* framebuffer = FramebufferAPI::Cast<DX11FramebufferData>(dataObject);
        DX11ContextData* context = ContextAPI::Cast<DX11ContextData>(framebuffer->Context);
        ID3D11DeviceContext* dxContext = DX11ContextAPI::GetNativeContext(context);
        DX11DepthTexture2DData* dxDepthTexture =
            DepthTexture2DAPI::Cast<DX11DepthTexture2DData>(framebuffer->m_DTAttachment);
        DX11RenderTexture2DData* dxRenderTexture = nullptr;

        for (UInt16 i = 0; i < framebuffer->m_ColorFormats.size(); i++) {
            if (!framebuffer->m_RTAttachments[i]) continue;

            dxRenderTexture = RenderTexture2DAPI::Cast<DX11RenderTexture2DData>(framebuffer->m_RTAttachments[i]);
            dxContext->ClearRenderTargetView(dxRenderTexture->m_NativeRTV, (Float*)&(clearColor));
        }

        if (dxDepthTexture) {
            dxContext->ClearDepthStencilView(
                dxDepthTexture->m_NativeDSV, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
        }
    }

    void DX11FramebufferAPI::Resize(FramebufferData* dataObject, const UInt32 width, const UInt32 height) {
        DX11FramebufferData* framebuffer = FramebufferAPI::Cast<DX11FramebufferData>(dataObject);
        Unbind(dataObject);
        Clean(dataObject);
        framebuffer->m_Width = width;
        framebuffer->m_Height = height;
    }

    void DX11FramebufferAPI::Clean(FramebufferData* dataObject) {
        RenderTexture2DData* renderTexture;
        for (UInt8 i = 0; i < 16; i++) {
            renderTexture = FramebufferAPI::GetRenderTexture(dataObject, i);
            if (renderTexture) RenderTexture2DAPI::Clean(renderTexture);
        }
    }
}  // namespace Sentinel