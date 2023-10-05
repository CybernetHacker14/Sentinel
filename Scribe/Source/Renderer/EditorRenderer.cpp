#include "Renderer/EditorRenderer.h"

#include <Sentinel/Window/Window.h>
#include <Sentinel/Graphics/Device/ContextAPI.h>
#include <Sentinel/Graphics/Device/SwapchainAPI.h>
#include <Sentinel/Graphics/Texture/RenderTexture2DAPI.h>

#include <glm/glm.hpp>

namespace Scribe {
    namespace Rendering {
        EditorRenderer::EditorRenderer(Sentinel::Window* window) : m_Window(window) {
            m_ResizeIndex = Sentinel::EventsAPI::RegisterEvent(
                Sentinel::EventType::WindowResize, this, ST_BIND_FN(EditorRenderer::OnWindowResize));

            m_CtxAlloc.Allocate(1);
            m_SCAlloc.Allocate(1);

            m_RTAlloc.Allocate(1);

            GLFWwindow* glfwWindow = static_cast<GLFWwindow*>(m_Window->GetNative());
            m_Context = Sentinel::ContextAPI::CreateImmediateContext(m_CtxAlloc, nullptr);
            m_Swapchain = Sentinel::SwapchainAPI::CreateSwapchain(m_SCAlloc, m_Context, glfwWindow);
        }

        EditorRenderer::~EditorRenderer() {
            Sentinel::EventsAPI::UnregisterEvent(Sentinel::EventType::WindowResize, m_ResizeIndex);
        }

        void EditorRenderer::OnAttach() {
            m_SwapchainRT = Sentinel::RenderTexture2DAPI::CreateRenderTexture2DData(m_RTAlloc, m_Context, m_Swapchain);

            Sentinel::RenderTexture2DAPI::Bind(m_SwapchainRT, 1, Sentinel::ShaderType::PIXEL);

            Sentinel::SwapchainAPI::SetBuffers(m_Swapchain, m_SwapchainRT, NULL);
            Resize(m_Window->GetWidth(), m_Window->GetHeight());
        }

        void EditorRenderer::OnDetach() {
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

        void EditorRenderer::OnUpdate() {
        }

        void EditorRenderer::OnRender() {
            Sentinel::SwapchainAPI::Bind(m_Swapchain);
            Sentinel::SwapchainAPI::SwapBuffers(m_Swapchain);
            //  Sentinel::RenderTexture2DAPI::Clear(m_SwapchainRT, {0.1f, 0.5f, 0.1f, 1.0f});
        }

        void EditorRenderer::OnPostRender() {
            Sentinel::SwapchainAPI::Unbind(m_Swapchain);
        }

        Sentinel::Bool EditorRenderer::OnWindowResize(
            Sentinel::EventType type, Sentinel::EventData data, void* listener) {
            EditorRenderer* renderer = (EditorRenderer*)listener;
            renderer->Resize(data.UInt16[0], data.UInt16[1]);
            return true;
        }

        void EditorRenderer::Resize(Sentinel::UInt16 width, Sentinel::UInt16 height) {
            Sentinel::RenderTexture2DAPI::Unbind(m_SwapchainRT);
            Sentinel::RenderTexture2DAPI::Clean(m_SwapchainRT);
            Sentinel::SwapchainAPI::Resize(m_Swapchain, width, height);
            Sentinel::RenderTexture2DAPI::Resize(m_SwapchainRT, width, height);
            Sentinel::RenderTexture2DAPI::Bind(m_SwapchainRT, 1, Sentinel::ShaderType::PIXEL);
        }
    }  // namespace Rendering
}  // namespace Scribe