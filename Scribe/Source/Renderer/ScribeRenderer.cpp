#pragma once

#include <Sentinel.h>

#include "Renderer/ScribeRenderer.h"

#include <Sentinel/Graphics/Common/Backend.h>
#include <Sentinel/Graphics/Common/GraphicsMemoryManager.h>

#include <Sentinel/Graphics/Device/ContextAPI.h>
#include <Sentinel/Graphics/Device/SwapchainAPI.h>
#include <Sentinel/Graphics/Output/FramebufferAPI.h>
#include <Sentinel/Graphics/Output/ViewportAPI.h>

#include <Sentinel/Graphics/Texture/RenderTexture2DAPI.h>
#include <Sentinel/Graphics/Texture/DepthTexture2DAPI.h>

#include <Sentinel/Graphics/Material/ShaderAPI.h>

namespace Scribe {
    namespace Rendering {
        Sentinel::Bool renderdocBuild = false;

        ScribeRenderer::ScribeRenderer(Sentinel::Window* window)
            : Sentinel::Layer("EditorRendererLayer"), m_Window(window) {
            m_AttachFunction = ST_BIND_EVENT_FN(ScribeRenderer::OnAttach);
            m_DetachFunction = ST_BIND_EVENT_FN(ScribeRenderer::OnDetach);
            m_UpdateFunction = ST_BIND_EVENT_FN(ScribeRenderer::OnUpdate);
            m_RenderFunction = ST_BIND_EVENT_FN(ScribeRenderer::OnRender);
            m_PostRenderFunction = ST_BIND_EVENT_FN(ScribeRenderer::OnPostRender);

            m_ResizeIndex = Sentinel::Application::Get().SubscribeToEvent(
                Sentinel::EventType::WindowResize, ST_BIND_EVENT_FN(ScribeRenderer::OnWindowResize));

            GLFWwindow* glfwWindow = m_Window->GetNativeWindow<GLFWwindow>();
            m_GFXMemory = Sentinel::CreateSharedRef<Sentinel::GraphicsMemoryManager>();
            m_Context = Sentinel::ContextAPI::CreateImmediateContext(m_GFXMemory, glfwWindow);
            m_Swapchain = Sentinel::SwapchainAPI::CreateSwapchain(m_GFXMemory, m_Context, glfwWindow);
        }

        ScribeRenderer::~ScribeRenderer() {
        }

        void ScribeRenderer::OnAttach() {
            m_SwapchainRT =
                Sentinel::RenderTexture2DAPI::CreateRenderTexture2DData(m_GFXMemory, m_Context, m_Swapchain);

            m_SwapchainDRT = Sentinel::DepthTexture2DAPI::CreateDepthTexture2DData(
                m_GFXMemory,
                m_Context,
                m_Window->GetWidth(),
                m_Window->GetHeight(),
                Sentinel::DepthFormat::D24S8UINT,
                true);

            if (renderdocBuild) {
                m_Shader =
                    Sentinel::ShaderAPI::CreateShaderData(m_GFXMemory, m_Context, "TextureShader.hlsl", "TexShader");

            } else {
                m_Shader = Sentinel::ShaderAPI::CreateShaderData(
                    m_GFXMemory, m_Context, "../Engine/Resources/Shaders/TextureShader.hlsl", "TexShader");
            }

            m_Viewport = Sentinel::ViewportAPI::CreateViewportData(
                m_GFXMemory, m_Context, 0, 0, m_Window->GetWidth(), m_Window->GetHeight(), 0, 1);

            Sentinel::ShaderAPI::Bind(m_Shader);
            Sentinel::RenderTexture2DAPI::Bind(m_SwapchainRT, 1, Sentinel::ShaderType::PIXEL);
            Sentinel::DepthTexture2DAPI::Bind(m_SwapchainDRT, 2, Sentinel::ShaderType::PIXEL);
            Sentinel::ViewportAPI::Bind(m_Viewport);

            Sentinel::SwapchainAPI::SetBuffers(m_Swapchain, m_SwapchainRT, m_SwapchainDRT);
            Resize(m_Window->GetWidth(), m_Window->GetHeight());
        }

        void ScribeRenderer::OnDetach() {
            Sentinel::ShaderAPI::Unbind(m_Shader);
            Sentinel::SwapchainAPI::UnsetBuffers(m_Swapchain);
            Sentinel::RenderTexture2DAPI::Unbind(m_SwapchainRT);
            Sentinel::DepthTexture2DAPI::Unbind(m_SwapchainDRT);
        }

        void ScribeRenderer::OnUpdate() {
        }

        void ScribeRenderer::OnRender() {
            Sentinel::SwapchainAPI::Bind(m_Swapchain);
            Sentinel::RenderTexture2DAPI::Clear(m_SwapchainRT, {0.1f, 0.5f, 0.1f, 1.0f});
            // Sentinel::ContextAPI::Draw(m_Context);
        }

        void ScribeRenderer::OnPostRender() {
            Sentinel::SwapchainAPI::SwapBuffers(m_Swapchain);
            Sentinel::SwapchainAPI::Unbind(m_Swapchain);
        }

        void ScribeRenderer::OnWindowResize(Sentinel::Event& event) {
            Sentinel::WindowResizeEvent e = *(event.Cast<Sentinel::WindowResizeEvent>());
            Resize(e.GetWidth(), e.GetHeight());
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