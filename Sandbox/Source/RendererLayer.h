#pragma once

#include <Sentinel.h>

namespace Sentinel {
    class Backend;
    class GraphicsMemoryManager;
    class Camera;

    struct ContextData;
    struct SwapchainData;
    struct FramebufferData;
    struct ViewportData;

    struct VertexbufferData;
    struct IndexbufferData;
    struct VertexbufferLayoutData;

    struct ShaderData;
    struct Texture2DData;

    struct RenderTexture2DData;
    struct DepthTexture2DData;

    class ImGuiLayer;
    class ImGuiDebugLayer;
}  // namespace Sentinel

namespace Sandbox {
    namespace Rendering {
        class RendererLayer final: public Sentinel::Layer {
        public:
            RendererLayer(Sentinel::Window* window);

        private:
            void OnAttach();
            void Detach();
            void OnUpdate();
            void OnRender();
            void OnImGuiRender();
            void OnPostRender();

            void OnWindowResize(Sentinel::Event& event);
            void Resize(Sentinel::UInt16 width, Sentinel::UInt16 height);

        public:
            Sentinel::ContextData* m_Context;

        private:
            Sentinel::SharedRef<Sentinel::GraphicsMemoryManager> m_GFXMemory;
            Sentinel::SharedRef<Sentinel::Camera> m_Camera;

            Sentinel::Window* m_Window;
            Sentinel::SwapchainData* m_Swapchain;
            Sentinel::FramebufferData* m_Framebuffer;
            Sentinel::ViewportData* m_Viewport;

            Sentinel::VertexbufferData* m_VBuffer;
            Sentinel::IndexbufferData* m_IBuffer;
            Sentinel::VertexbufferLayoutData* m_VLayout;

            Sentinel::ShaderData* m_Shader;
            Sentinel::Texture2DData* m_Texture;

            Sentinel::RenderTexture2DData* m_RenderTexture;
            Sentinel::DepthTexture2DData* m_DepthTexture;

        private:
            Sentinel::UInt32 m_ResizeIndex = 0;
        };
    }  // namespace Rendering
}  // namespace Sandbox