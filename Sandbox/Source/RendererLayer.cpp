#include "RendererLayer.h"

#include <Sentinel/Graphics/Common/Backend.h>
#include <Sentinel/Graphics/Common/GraphicsMemoryManager.h>
#include <Sentinel/Graphics/Camera/Camera.h>

#include <Sentinel/Graphics/Device/ContextAPI.h>
#include <Sentinel/Graphics/Device/SwapchainAPI.h>
#include <Sentinel/Graphics/Output/FramebufferAPI.h>
#include <Sentinel/Graphics/Output/ViewportAPI.h>

#include <Sentinel/Graphics/Buffer/VertexbufferAPI.h>
#include <Sentinel/Graphics/Buffer/IndexbufferAPI.h>
#include <Sentinel/Graphics/Buffer/VertexbufferLayoutAPI.h>

#include <Sentinel/Graphics/Material/ShaderAPI.h>

#include <Sentinel/Graphics/Texture/Texture2DAPI.h>
#include <Sentinel/Graphics/Texture/RenderTexture2DAPI.h>
#include <Sentinel/Graphics/Texture/DepthTexture2DAPI.h>

#include <glm/glm.hpp>

namespace Sandbox {
    namespace Rendering {
        RendererLayer::RendererLayer(Sentinel::Window* window) : m_Window(window) {
            m_AttachFunction = ST_BIND_EVENT_FN(RendererLayer::OnAttach);
            m_DetachFunction = ST_BIND_EVENT_FN(RendererLayer::OnDetach);
            m_UpdateFunction = ST_BIND_EVENT_FN(RendererLayer::OnUpdate);
            m_RenderFunction = ST_BIND_EVENT_FN(RendererLayer::OnRender);
            m_ImGuiRenderFunction = ST_BIND_EVENT_FN(RendererLayer::OnImGuiRender);
            m_PostRenderFunction = ST_BIND_EVENT_FN(RendererLayer::OnPostRender);

            m_ResizeIndex = Sentinel::Application::Get().SubscribeToEvent(
                Sentinel::EventType::WindowResize, ST_BIND_EVENT_FN(RendererLayer::OnWindowResize));

            GLFWwindow* glfwWindow = m_Window->GetNativeWindow<GLFWwindow>();
            m_GFXMemory = Sentinel::CreateSharedRef<Sentinel::GraphicsMemoryManager>();
            m_Context = Sentinel::ContextAPI::CreateImmediateContext(m_GFXMemory, glfwWindow);
            m_Swapchain = Sentinel::SwapchainAPI::CreateSwapchain(m_GFXMemory, m_Context, glfwWindow);
            m_Camera = Sentinel::CreateSharedRef<Sentinel::Camera>(
                m_GFXMemory, m_Context, m_Window->GetWidth(), m_Window->GetHeight());
        }

        void RendererLayer::OnAttach() {
            Sentinel::STL::vector<Sentinel::STL::pair<glm::vec4, glm::vec2>> vertices = {
                {{-3.0f, 3.0f, -7.0f, 1.0f}, {0.0f, 0.0f}},
                {{-1.0f, 3.0f, -7.0f, 1.0f}, {1.0f, 0.0f}},
                {{-1.0f, 1.0f, -7.0f, 1.0f}, {1.0f, 1.0f}},
                {{-3.0f, 1.0f, -7.0f, 1.0f}, {0.0f, 1.0f}},

                {{1.0f, 1.0f, -5.0f, 1.0f}, {0.0f, 0.0f}},
                {{3.0f, 1.0f, -5.0f, 1.0f}, {1.0f, 0.0f}},
                {{3.0f, -1.0f, -5.0f, 1.0f}, {1.0f, 1.0f}},
                {{1.0f, -1.0f, -5.0f, 1.0f}, {0.0f, 1.0f}}};

            Sentinel::STL::vector<Sentinel::UInt32> indices = {0, 1, 2, 0, 2, 3, 4, 5, 6, 4, 6, 7};

            m_VLayout = Sentinel::VertexbufferLayoutAPI::CreateVertexbufferLayoutData(m_GFXMemory, m_Context);

            m_VBuffer = Sentinel::VertexbufferAPI::CreateVertexbufferData(
                m_GFXMemory,
                m_Context,
                vertices.data(),
                vertices.size() * sizeof(Sentinel::STL::pair<glm::vec4, glm::vec2>));

            m_IBuffer =
                Sentinel::IndexbufferAPI::CreateIndexbufferData(m_GFXMemory, m_Context, indices.data(), indices.size());

            m_Shader = Sentinel::ShaderAPI::CreateShaderData(
                m_GFXMemory, m_Context, "../Engine/Resources/Shaders/TextureShader.hlsl", "TexShader");

            Sentinel::VertexbufferLayoutAPI::CreateLayout(m_VLayout, m_Shader);

            Sentinel::Texture2DDataImportSettings settings;
            settings.TextureFilepath = "../Engine/Resources/Images/Icon/512.png";

            m_Texture = Sentinel::Texture2DAPI::CreateTexture2DData(m_GFXMemory, m_Context, settings);

            m_RenderTexture =
                Sentinel::RenderTexture2DAPI::CreateRenderTexture2DData(m_GFXMemory, m_Context, m_Swapchain);

            m_DepthTexture = Sentinel::DepthTexture2DAPI::CreateDepthTexture2DData(
                m_GFXMemory,
                m_Context,
                m_Window->GetWidth(),
                m_Window->GetHeight(),
                Sentinel::DepthFormat::D24S8UINT,
                true);

            Sentinel::SwapchainAPI::SetBuffers(m_Swapchain, m_RenderTexture, m_DepthTexture);

            m_Viewport = Sentinel::ViewportAPI::CreateViewportData(
                m_GFXMemory, m_Context, 0, 0, m_Window->GetWidth(), m_Window->GetHeight(), 0, 1);

            Sentinel::VertexbufferLayoutAPI::Bind(m_VLayout);
            Sentinel::VertexbufferAPI::Bind(m_VBuffer, Sentinel::VertexbufferLayoutAPI::GetStride(m_VLayout));
            Sentinel::IndexbufferAPI::Bind(m_IBuffer);
            Sentinel::ShaderAPI::Bind(m_Shader);
            Sentinel::Texture2DAPI::Bind(m_Texture, 0, Sentinel::ShaderType::PIXEL);
            Sentinel::RenderTexture2DAPI::Bind(m_RenderTexture, 1, Sentinel::ShaderType::PIXEL);
            Sentinel::DepthTexture2DAPI::Bind(m_DepthTexture, 2, Sentinel::ShaderType::PIXEL);
            Sentinel::ViewportAPI::Bind(m_Viewport);
            Resize(m_Window->GetWidth(), m_Window->GetHeight());
        }

        void RendererLayer::Detach() {
            Sentinel::SwapchainAPI::Unbind(m_Swapchain);
            Sentinel::VertexbufferAPI::Unbind(m_VBuffer);
            Sentinel::IndexbufferAPI::Unbind(m_IBuffer);
            Sentinel::VertexbufferLayoutAPI::Unbind(m_VLayout);
            Sentinel::ShaderAPI::Unbind(m_Shader);
            Sentinel::Texture2DAPI::Unbind(m_Texture);
            Sentinel::SwapchainAPI::UnsetBuffers(m_Swapchain);
            Sentinel::RenderTexture2DAPI::Unbind(m_RenderTexture);
            Sentinel::DepthTexture2DAPI::Unbind(m_DepthTexture);

            Sentinel::ContextAPI::Clean(m_Context);
        }

        void RendererLayer::OnUpdate() {
            m_Camera->OnUpdate();
        }

        void RendererLayer::OnRender() {
            Sentinel::SwapchainAPI::Bind(m_Swapchain);
            Sentinel::ContextAPI::DrawIndexed(m_Context, Sentinel::IndexbufferAPI::GetCount(m_IBuffer));
            Sentinel::SwapchainAPI::SwapBuffers(m_Swapchain);
            Sentinel::RenderTexture2DAPI::Clear(m_RenderTexture, {0.1f, 0.8f, 0.1f, 1.0f});
            Sentinel::DepthTexture2DAPI::Clear(m_DepthTexture);
        }

        void RendererLayer::OnImGuiRender() {
        }

        void RendererLayer::OnPostRender() {
            Sentinel::SwapchainAPI::Unbind(m_Swapchain);
        }

        void RendererLayer::OnWindowResize(Sentinel::Event& event) {
            Sentinel::WindowResizeEvent e = *(event.Cast<Sentinel::WindowResizeEvent>());
            Resize(e.GetWidth(), e.GetHeight());
            m_Camera->OnResize(e.GetWidth(), e.GetHeight());
        }

        void RendererLayer::Resize(Sentinel::UInt16 width, Sentinel::UInt16 height) {
            Sentinel::RenderTexture2DAPI::Unbind(m_RenderTexture);
            Sentinel::DepthTexture2DAPI::Unbind(m_DepthTexture);
            Sentinel::RenderTexture2DAPI::Clean(m_RenderTexture);
            Sentinel::DepthTexture2DAPI::Clean(m_DepthTexture);
            Sentinel::SwapchainAPI::Resize(m_Swapchain, width, height);
            Sentinel::RenderTexture2DAPI::Resize(m_RenderTexture, width, height);
            Sentinel::DepthTexture2DAPI::Resize(m_DepthTexture, width, height);
            Sentinel::ViewportAPI::Resize(m_Viewport, width, height);
            Sentinel::ViewportAPI::Bind(m_Viewport);
            Sentinel::RenderTexture2DAPI::Bind(m_RenderTexture, 1, Sentinel::ShaderType::PIXEL);
            Sentinel::DepthTexture2DAPI::Bind(m_DepthTexture, 2, Sentinel::ShaderType::PIXEL);
        }
    }  // namespace Rendering
}  // namespace Sandbox