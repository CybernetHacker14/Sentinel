#include "stpch.h"

#ifdef ST_RENDERER_DX11

    #include "Sentinel/Graphics/Output/FramebufferAPI.h"
    #include "Sentinel/Graphics/Device/ContextAPI.h"
    #include "Sentinel/Graphics/Texture/RenderTexture2DAPI.h"
    #include "Sentinel/Graphics/Texture/DepthTexture2DAPI.h"

    #include "Platform/Windows/DirectX11/Graphics/Core/DX11Common.h"

    #include <glm/glm.hpp>

namespace Sentinel {

    FramebufferData* Sentinel::FramebufferAPI::CreateFramebufferData(
        FixedSlabAllocator<FramebufferData>& allocator, ContextData* context, UInt16 width, UInt16 height) {
        FramebufferData* framebuffer = allocator.New();
        framebuffer->Context = context;
        framebuffer->m_Width = width;
        framebuffer->m_Height = height;

        return framebuffer;
    }

    void FramebufferAPI::CreateAttachments(
        FramebufferData* dataObject,
        FixedSlabAllocator<RenderTexture2DData>& rtAllocator,
        FixedSlabAllocator<DepthTexture2DData>& dtAllocator) {
        for (UInt8 i = 0; i < dataObject->m_ColorFormats.Size(); i++) {
            dataObject->m_RTAttachments[i] = RenderTexture2DAPI::CreateRenderTexture2DData(
                rtAllocator,
                dataObject->Context,
                dataObject->m_Width,
                dataObject->m_Height,
                dataObject->m_ColorFormats[i]);
        }

        dataObject->m_DTAttachment = DepthTexture2DAPI::CreateDepthTexture2DData(
            dtAllocator, dataObject->Context, dataObject->m_Width, dataObject->m_Height, dataObject->m_DepthFormat);
    }

    void FramebufferAPI::Bind(FramebufferData* dataObject) {
        ID3D11DeviceContext* nativeContext = ContextAPI::GetNativeContext(dataObject->Context);

        Microsoft::WRL::ComPtr<ID3D11RenderTargetView> pRenderViews[5];

        for (UInt8 i = 0; i < dataObject->m_ColorFormats.Size(); i++) {
            pRenderViews[i] = RenderTexture2DAPI::GetNativeRTV(dataObject->m_RTAttachments[i]);
        }

        nativeContext->OMSetRenderTargets(
            dataObject->m_ColorFormats.Size(),
            pRenderViews[0].GetAddressOf(),
            dataObject->m_DepthFormat == DepthFormat::NONE
                ? NULL
                : DepthTexture2DAPI::GetNativeDSV(dataObject->m_DTAttachment));
    }

    void FramebufferAPI::Unbind(FramebufferData* dataObject) {
        ID3D11RenderTargetView* null = nullptr;
        ContextAPI::GetNativeContext(dataObject->Context)->OMSetRenderTargets(1, &null, NULL);
    }

    void FramebufferAPI::Clear(FramebufferData* dataObject, const glm::vec4& clearColor) {
        ID3D11DeviceContext* dxContext = ContextAPI::GetNativeContext(dataObject->Context);

        for (UInt16 i = 0; i < dataObject->m_ColorFormats.Size(); i++) {
            if (!dataObject->m_RTAttachments[i]) continue;

            dxContext->ClearRenderTargetView(
                RenderTexture2DAPI::GetNativeRTV(dataObject->m_RTAttachments[i]), (Float*)&(clearColor));
        }

        if (dataObject->m_DepthFormat != DepthFormat::NONE) {
            dxContext->ClearDepthStencilView(
                DepthTexture2DAPI::GetNativeDSV(dataObject->m_DTAttachment),
                D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,
                1.0f,
                0);
        }
    }

    void FramebufferAPI::Resize(FramebufferData* dataObject, const UInt16 width, const UInt16 height) {
        Unbind(dataObject);
        // Clean(dataObject);
        RenderTexture2DData* renderTexture;
        for (UInt8 i = 0; i < dataObject->m_ColorFormats.Size(); i++) {
            renderTexture = GetRenderTexture(dataObject, i);
            if (renderTexture) RenderTexture2DAPI::Resize(renderTexture, width, height);
        }
        dataObject->m_Width = width;
        dataObject->m_Height = height;
    }

    void FramebufferAPI::Clean(FramebufferData* dataObject) {
        RenderTexture2DData* renderTexture;
        for (UInt8 i = 0; i < dataObject->m_ColorFormats.Size(); i++) {
            renderTexture = GetRenderTexture(dataObject, i);
            if (renderTexture) RenderTexture2DAPI::Clean(renderTexture);
        }

        if (dataObject->m_DTAttachment) DepthTexture2DAPI::Clean(dataObject->m_DTAttachment);
    }

}  // namespace Sentinel
#endif  // ST_RENDERER_DX11