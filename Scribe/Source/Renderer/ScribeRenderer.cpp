#include "Renderer/ScribeRenderer.h"

#include <Sentinel/Window/Window.h>

#include <glm/glm.hpp>

namespace Scribe {
    namespace Rendering {
        ScribeRenderer::ScribeRenderer(Sentinel::Window* window) : m_Window(window) {
            m_ResizeIndex = Sentinel::EventsAPI::RegisterEvent(
                Sentinel::EventType::WindowResize, this, ST_BIND_FN(ScribeRenderer::OnWindowResize));

            m_CtxAlloc.Allocate(1);
            m_SCAlloc.Allocate(1);

            m_RTAlloc.Allocate(1);

            GLFWwindow* glfwWindow = static_cast<GLFWwindow*>(m_Window->GetNative());
            m_Context = Sentinel::ContextAPI::CreateImmediateContext(m_CtxAlloc, glfwWindow);
            m_Swapchain = Sentinel::SwapchainAPI::CreateSwapchain(m_SCAlloc, m_Context, glfwWindow);
        }

        ScribeRenderer::~ScribeRenderer() {
            Sentinel::EventsAPI::UnregisterEvent(Sentinel::EventType::WindowResize, m_ResizeIndex);
        }

        void ScribeRenderer::OnAttach() {
            m_SwapchainRT = Sentinel::RenderTexture2DAPI::CreateRenderTexture2DData(m_RTAlloc, m_Context, m_Swapchain);

            Sentinel::RenderTexture2DAPI::Bind(m_SwapchainRT, 1, Sentinel::ShaderType::PIXEL);

            Sentinel::SwapchainAPI::SetBuffers(m_Swapchain, m_SwapchainRT, NULL);
            Resize(m_Window->GetWidth(), m_Window->GetHeight());
        }

        void ScribeRenderer::OnDetach() {
            Sentinel::SwapchainAPI::Unbind(m_Swapchain);
            Sentinel::SwapchainAPI::UnsetBuffers(m_Swapchain);
            Sentinel::RenderTexture2DAPI::Unbind(m_SwapchainRT);

            Sentinel::RenderTexture2DAPI::Clean(m_SwapchainRT);

            Sentinel::SwapchainAPI::Clean(m_Swapchain);
            Sentinel::ContextAPI::Clean(m_Context);

            m_RTAlloc.DeleteAll();

            m_SCAlloc.DeleteAll();
            m_CtxAlloc.DeleteAll();

            m_RTAlloc.Deallocate();

            m_SCAlloc.Deallocate();
            m_CtxAlloc.Deallocate();
        }

        void ScribeRenderer::OnUpdate() {
        }

        void ScribeRenderer::OnRender() {
            Sentinel::SwapchainAPI::Bind(m_Swapchain);
            Sentinel::SwapchainAPI::SwapBuffers(m_Swapchain);
            Sentinel::RenderTexture2DAPI::Clear(m_SwapchainRT, {0.1f, 0.5f, 0.1f, 1.0f});
        }

        void ScribeRenderer::OnPostRender() {
            Sentinel::SwapchainAPI::Unbind(m_Swapchain);
        }

        Sentinel::Bool ScribeRenderer::OnWindowResize(
            Sentinel::EventType type, Sentinel::EventData data, void* listener) {
            ScribeRenderer* renderer = (ScribeRenderer*)listener;
            renderer->Resize(data.UInt16[0], data.UInt16[1]);
            return true;
        }

        void ScribeRenderer::Resize(Sentinel::UInt16 width, Sentinel::UInt16 height) {
            Sentinel::RenderTexture2DAPI::Unbind(m_SwapchainRT);
            Sentinel::RenderTexture2DAPI::Clean(m_SwapchainRT);
            Sentinel::SwapchainAPI::Resize(m_Swapchain, width, height);
            Sentinel::RenderTexture2DAPI::Resize(m_SwapchainRT, width, height);
            Sentinel::RenderTexture2DAPI::Bind(m_SwapchainRT, 1, Sentinel::ShaderType::PIXEL);
        }
    }  // namespace Rendering
}  // namespace Scribe