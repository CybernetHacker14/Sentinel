#include "Renderer/ScribeRenderer.h"

#include <Sentinel/Graphics/Camera/Camera.h>
#include <Sentinel/Window/Window.h>
#include <Sentinel/Logging/Log.h>

#include <glm/glm.hpp>

namespace Scribe {
    namespace Rendering {
        ScribeRenderer::ScribeRenderer(Sentinel::Window* window) : m_Window(window) {
            m_ResizeIndex = Sentinel::EventsAPI::RegisterEvent(
                Sentinel::EventType::WindowResize, this, ST_BIND_FN(ScribeRenderer::OnWindowResize));

            m_CtxAlloc.Allocate(1);
            m_SCAlloc.Allocate(1);
            m_VPortAlloc.Allocate(1);

            m_ShaderAlloc.Allocate(1);

            m_RTAlloc.Allocate(1);
            m_DTAlloc.Allocate(1);

            GLFWwindow* glfwWindow = static_cast<GLFWwindow*>(m_Window->GetNative());
            m_Context = Sentinel::ContextAPI::CreateImmediateContext(m_CtxAlloc, glfwWindow);
            m_Swapchain = Sentinel::SwapchainAPI::CreateSwapchain(m_SCAlloc, m_Context, glfwWindow);
        }

        ScribeRenderer::~ScribeRenderer() {
            Sentinel::EventsAPI::UnregisterEvent(Sentinel::EventType::WindowResize, m_ResizeIndex);
        }

        void ScribeRenderer::OnAttach() {
            const Sentinel::ContextInfo& info = Sentinel::ContextAPI::GetContextInfo(m_Context);
            ST_TERMINAL_INFO("Vendor   : %s", info.Vendor);
            ST_TERMINAL_INFO("Renderer : %s", info.Renderer);
            ST_TERMINAL_INFO("API      : %s", info.API);
            ST_TERMINAL_INFO("Version  : %s", info.Version);

            m_SwapchainRT = Sentinel::RenderTexture2DAPI::CreateRenderTexture2DData(m_RTAlloc, m_Context, m_Swapchain);

            m_SwapchainDRT = Sentinel::DepthTexture2DAPI::CreateDepthTexture2DData(
                m_DTAlloc,
                m_Context,
                m_Window->GetWidth(),
                m_Window->GetHeight(),
                Sentinel::DepthFormat::D24S8UINT,
                true);

            m_Shader = Sentinel::ShaderAPI::CreateShaderData(
                m_ShaderAlloc, m_Context, "Assets/Shaders/Base.hlsl", "BaseShader");

            m_Viewport = Sentinel::ViewportAPI::CreateViewportData(
                m_VPortAlloc, m_Context, 0, 0, m_Window->GetWidth(), m_Window->GetHeight(), 0, 1);

            Sentinel::ShaderAPI::Bind(m_Shader);
            Sentinel::RenderTexture2DAPI::Bind(m_SwapchainRT, 1, Sentinel::ShaderType::PIXEL);
            Sentinel::DepthTexture2DAPI::Bind(m_SwapchainDRT, 2, Sentinel::ShaderType::PIXEL);
            Sentinel::ViewportAPI::Bind(m_Viewport);

            Sentinel::SwapchainAPI::SetBuffers(m_Swapchain, m_SwapchainRT, m_SwapchainDRT);
            Resize(m_Window->GetWidth(), m_Window->GetHeight());
        }

        void ScribeRenderer::OnDetach() {
            Sentinel::SwapchainAPI::Unbind(m_Swapchain);
            Sentinel::ShaderAPI::Unbind(m_Shader);
            Sentinel::SwapchainAPI::UnsetBuffers(m_Swapchain);
            Sentinel::RenderTexture2DAPI::Unbind(m_SwapchainRT);
            Sentinel::DepthTexture2DAPI::Unbind(m_SwapchainDRT);

            // m_Camera->Clean();
            Sentinel::ShaderAPI::Clean(m_Shader);

            Sentinel::RenderTexture2DAPI::Clean(m_SwapchainRT);
            Sentinel::DepthTexture2DAPI::Clean(m_SwapchainDRT);

            Sentinel::SwapchainAPI::Clean(m_Swapchain);
            Sentinel::ContextAPI::Clean(m_Context);

            m_ShaderAlloc.DeleteAll();

            m_RTAlloc.DeleteAll();
            m_DTAlloc.DeleteAll();

            m_VPortAlloc.DeleteAll();
            m_SCAlloc.DeleteAll();
            m_CtxAlloc.DeleteAll();

            m_ShaderAlloc.Deallocate();

            m_RTAlloc.Deallocate();
            m_DTAlloc.Deallocate();

            m_VPortAlloc.Deallocate();
            m_SCAlloc.Deallocate();
            m_CtxAlloc.Deallocate();
        }

        void ScribeRenderer::OnUpdate() {
        }

        void ScribeRenderer::OnRender() {
            Sentinel::SwapchainAPI::Bind(m_Swapchain);
            // Sentinel::ContextAPI::Draw(m_Context);
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
            Sentinel::DepthTexture2DAPI::Unbind(m_SwapchainDRT);
            Sentinel::RenderTexture2DAPI::Clean(m_SwapchainRT);
            Sentinel::DepthTexture2DAPI::Clean(m_SwapchainDRT);
            Sentinel::SwapchainAPI::Resize(m_Swapchain, width, height);
            Sentinel::RenderTexture2DAPI::Resize(m_SwapchainRT, width, height);
            Sentinel::DepthTexture2DAPI::Resize(m_SwapchainDRT, width, height);
            Sentinel::ViewportAPI::Resize(m_Viewport, width, height);
            Sentinel::ViewportAPI::Bind(m_Viewport);
            Sentinel::RenderTexture2DAPI::Bind(m_SwapchainRT, 1, Sentinel::ShaderType::PIXEL);
            Sentinel::DepthTexture2DAPI::Bind(m_SwapchainDRT, 2, Sentinel::ShaderType::PIXEL);
        }
    }  // namespace Rendering
}  // namespace Scribe