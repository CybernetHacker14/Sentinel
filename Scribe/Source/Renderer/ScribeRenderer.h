#pragma once

#include <Sentinel.h>

namespace Sentinel {
    class Backend;
    class GraphicsMemoryManager;

    struct ContextData;
    struct SwapchainData;
    struct FramebufferData;
    struct ViewportData;

    struct ShaderData;

    struct RenderTexture2DData;
    struct DepthTexture2DData;
}  // namespace Sentinel

namespace Scribe {
    namespace Rendering {
        class ScribeRenderer final: public Sentinel::Layer {
        public:
            ScribeRenderer(Sentinel::Window* window);
            ~ScribeRenderer();

            inline Sentinel::ContextData* GetRenderingContext() { return m_Context; }

        private:
            void OnAttach();
            void OnDetach();
            void OnUpdate();
            void OnRender();
            void OnPostRender();

            void OnWindowResize(Sentinel::Event& event);
            void Resize(Sentinel::UInt16 width, Sentinel::UInt16 height);

        private:
            Sentinel::SharedRef<Sentinel::GraphicsMemoryManager> m_GFXMemory;

            Sentinel::Window* m_Window;

            Sentinel::ContextData* m_Context;
            Sentinel::SwapchainData* m_Swapchain;
            Sentinel::FramebufferData* m_Framebuffer;
            Sentinel::ViewportData* m_Viewport;

            Sentinel::RenderTexture2DData* m_SwapchainRT;
            Sentinel::DepthTexture2DData* m_SwapchainDRT;

            Sentinel::ShaderData* m_Shader;

        private:
            Sentinel::UInt32 m_ResizeIndex = 0;
        };
    }  // namespace Rendering
}  // namespace Scribe