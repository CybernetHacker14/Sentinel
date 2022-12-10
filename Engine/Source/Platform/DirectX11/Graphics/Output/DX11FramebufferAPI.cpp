#include "stpch.h"

#if ST_RENDERER_DX11

    #include "Sentinel/Graphics/Output/FramebufferAPI.h"
    #include "Sentinel/Graphics/Device/ContextAPI.h"
    #include "Sentinel/Graphics/Texture/RenderTexture2DAPI.h"
    #include "Sentinel/Graphics/Texture/DepthTexture2DAPI.h"

    #include "Platform/DirectX11/Graphics/Core/DX11Common.h"

    #include <glm/glm.hpp>

namespace Sentinel {

    FramebufferData* Sentinel::FramebufferAPI::CreateFramebufferData(
        PoolAllocator<FramebufferData>& allocator,
        PoolAllocator<RenderTexture2DData>& rtAllocator,
        PoolAllocator<DepthTexture2DData>& dtAllocator,
        ContextData* context,
        UInt16 width,
        UInt16 height) {
        FramebufferData* framebuffer = allocator.New();
        framebuffer->Context = context;

        for (UInt8 i = 0; i < framebuffer->m_ColorFormats.size(); i++) {
            framebuffer->m_RTAttachments[i] = RenderTexture2DAPI::CreateRenderTexture2DData(
                rtAllocator,
                framebuffer->Context,
                framebuffer->m_Width,
                framebuffer->m_Height,
                framebuffer->m_ColorFormats[i]);
        }

        framebuffer->m_DTAttachment = DepthTexture2DAPI::CreateDepthTexture2DData(
            dtAllocator, framebuffer->Context, framebuffer->m_Width, framebuffer->m_Height, framebuffer->m_DepthFormat);

        return framebuffer;
    }

    void FramebufferAPI::Bind(FramebufferData* dataObject) {
        ID3D11DeviceContext* nativeContext = ContextAPI::GetNativeContext(dataObject->Context);

        Microsoft::WRL::ComPtr<ID3D11RenderTargetView> pRenderViews[15];

        for (UInt8 i = 0; i < dataObject->m_ColorFormats.size(); i++) {
            pRenderViews[i] = RenderTexture2DAPI::GetNativeRTV(dataObject->m_RTAttachments[i]);
        }

        nativeContext->OMSetRenderTargets(
            dataObject->m_ColorFormats.size(),
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

        for (UInt16 i = 0; i < dataObject->m_ColorFormats.size(); i++) {
            if (!dataObject->m_RTAttachments[i]) continue;

            dxContext->ClearRenderTargetView(
                RenderTexture2DAPI::GetNativeRTV(dataObject->m_RTAttachments[i]), (Float*)&(clearColor));
        }

        if (dataObject->m_DTAttachment) {
            dxContext->ClearDepthStencilView(
                DepthTexture2DAPI::GetNativeDSV(dataObject->m_DTAttachment),
                D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,
                1.0f,
                0);
        }
    }

    void FramebufferAPI::Resize(FramebufferData* dataObject, const UInt16 width, const UInt16 height) {
        Unbind(dataObject);
        Clean(dataObject);
        dataObject->m_Width = width;
        dataObject->m_Height = height;
    }

    void FramebufferAPI::Clean(FramebufferData* dataObject) {
        RenderTexture2DData* renderTexture;
        for (UInt8 i = 0; i < 16; i++) {
            renderTexture = GetRenderTexture(dataObject, i);
            if (renderTexture) RenderTexture2DAPI::Clean(renderTexture);
        }

        if (dataObject->m_DTAttachment) DepthTexture2DAPI::Clean(dataObject->m_DTAttachment);
    }

}  // namespace Sentinel
#endif  // ST_RENDERER_DX11