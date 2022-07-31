#include "stpch.h"
#include "Platform/DirectX11/Graphics/Core/DX11Common.h"
#include "Platform/DirectX11/Graphics/Modules/DX11RenderPipeline.h"

namespace Sentinel {
    DX11RenderPipeline::DX11RenderPipeline() {
        m_CreateFrameBackingsFunction = ST_BIND_EVENT_FN(CreateFrameBackings);
        m_CreateFrameBackingsComponentsFunction = ST_BIND_EVENT_FN(CreateFrameBackingsComponents);
        m_InitFrameBackingsFunction = ST_BIND_EVENT_FN(InitFrameBackings);
        m_BindRenderResourcesFunction = ST_BIND_EVENT_FN(BindRenderResources);
        m_BindFramebufferFunction = ST_BIND_EVENT_FN(BindFramebuffer);

        m_ResizeFunction = ST_BIND_EVENT_FN(Resize);
        m_DrawFunction = ST_BIND_EVENT_FN(Draw);
        m_SwapBuffersFunction = ST_BIND_EVENT_FN(SwapBuffers);
        m_ClearFunction = ST_BIND_EVENT_FN(Clear);

        m_UnbindFramebufferFunction = ST_BIND_EVENT_FN(UnbindFramebuffer);
        m_UnbindRenderResourcesFunction = ST_BIND_EVENT_FN(UnbindRenderResources);
        m_CleanRenderResourcesFunction = ST_BIND_EVENT_FN(CleanRenderResources);
        m_CleanFrameBackingsFunction = ST_BIND_EVENT_FN(CleanFrameBackings);
        m_CleanNativeResourcesFunction = ST_BIND_EVENT_FN(CleanNativeResources);
    }

    void DX11RenderPipeline::CreateFrameBackings(
        const WindowProperties& windowProps, const FramebufferSpecification& framebufferSpecs) {
        m_FrameBackings = FrameBackings::Create();
        m_FrameBackings->WindowProperties = windowProps;
        m_FrameBackings->FramebufferSpecification = framebufferSpecs;
    }

    void DX11RenderPipeline::CreateFrameBackingsComponents() {
        m_FrameBackings->Window = Window::Create(m_FrameBackings->WindowProperties);
        m_FrameBackings->GraphicsContext =
            GraphicsContext::Create(m_FrameBackings->Window->GetNativeWindow<GLFWwindow>());
        m_FrameBackings->Framebuffer = Framebuffer::Create(m_FrameBackings->FramebufferSpecification);
    }

    void DX11RenderPipeline::InitFrameBackings() {
        m_FrameBackings->Window->SetVSync(true);
        m_FrameBackings->GraphicsContext->Init();
        ContextInfo info = m_FrameBackings->GraphicsContext->GetContextInfo();

        ST_ENGINE_INFO("VENDOR   : {0}", info.Vendor.c_str());
        ST_ENGINE_INFO("RENDERER : {0}", info.Renderer.c_str());
        ST_ENGINE_INFO("API      : {0}", info.API.c_str());
        ST_ENGINE_INFO("VERSION  : {0}", info.Version.c_str());
    }

    void DX11RenderPipeline::BindRenderResources() {
    }

    void DX11RenderPipeline::BindFramebuffer() { m_FrameBackings->Framebuffer->Bind(); }

    void DX11RenderPipeline::Resize(UInt32 width, UInt32 height) {
        m_FrameBackings->Framebuffer->Resize(width, height);
    }

    void DX11RenderPipeline::Draw() {
    }

    void DX11RenderPipeline::SwapBuffers(Bool vSync) { DX11Common::GetSwapchain()->Present(vSync ? 1 : 0, 0); }

    void DX11RenderPipeline::Clear() { m_FrameBackings->Framebuffer->Clear(); }

    void DX11RenderPipeline::UnbindFramebuffer() { m_FrameBackings->Framebuffer->Unbind(); }

    void DX11RenderPipeline::UnbindRenderResources() {
    }

    void DX11RenderPipeline::CleanRenderResources() {
    }

    void DX11RenderPipeline::CleanFrameBackings() {
        m_FrameBackings->Framebuffer->Clean();
        m_FrameBackings->Window->Shutdown();
    }

    void DX11RenderPipeline::CleanNativeResources() {
        DX11Common::m_Device->Release();
        DX11Common::m_Context->Release();
        DX11Common::m_DXGIDevice->Release();
        DX11Common::m_Adapter->Release();
        DX11Common::m_Swapchain->Release();
    }
}  // namespace Sentinel
