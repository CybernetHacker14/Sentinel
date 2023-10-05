#include "Renderer/SceneRenderer.h"

#include <Sentinel/Window/Window.h>

#include <Sentinel/Graphics/Device/ContextAPI.h>
#include <Sentinel/Graphics/Output/ViewportAPI.h>

#include <Sentinel/Graphics/Output/FramebufferAPI.h>

#include <Sentinel/Graphics/Buffer/VertexbufferAPI.h>
#include <Sentinel/Graphics/Buffer/IndexbufferAPI.h>
#include <Sentinel/Graphics/Buffer/VertexbufferLayoutAPI.h>
#include <Sentinel/Graphics/Buffer/ConstantbufferAPI.h>

#include <Sentinel/Graphics/Material/ShaderAPI.h>

#include <Sentinel/Graphics/Texture/Texture2DAPI.h>
#include <Sentinel/Graphics/Texture/RenderTexture2DAPI.h>
#include <Sentinel/Graphics/Texture/DepthTexture2DAPI.h>

#include <Sentinel/Graphics/Camera/Camera.h>

#include <vector>

namespace Scribe {
    namespace Rendering {
        SceneRenderer::SceneRenderer(
            Sentinel::Window* window, Sentinel::ContextData* context, Sentinel::SwapchainData* swapchain)
            : m_Window(window), m_Context(context), m_Swapchain(swapchain) {
            /*m_ResizeIndex = Sentinel::EventsAPI::RegisterEvent(
                Sentinel::EventType::WindowResize, this, ST_BIND_FN(SceneRenderer::OnWindowResize));*/

            m_VPortAlloc.Allocate(1);

            m_FBufferAlloc.Allocate(1);

            m_VBufferAlloc.Allocate(1);
            m_IBufferAlloc.Allocate(1);
            m_VLayoutAlloc.Allocate(1);
            m_CBufferAlloc.Allocate(1);

            m_ShaderAlloc.Allocate(1);
            m_TexAlloc.Allocate(1);

            m_RTAlloc.Allocate(1);
            m_DTAlloc.Allocate(1);

            m_CamCBuffer = Sentinel::ConstantbufferAPI::CreateConstantbufferData(
                m_CBufferAlloc, m_Context, sizeof(glm::mat4), 0, Sentinel::CBufferUsageType::DYNAMIC);
            m_Camera = new Sentinel::Camera(m_Window->GetWidth(), m_Window->GetHeight());
        }

        SceneRenderer::~SceneRenderer() {
            delete m_Camera;
        }

        Sentinel::RenderTexture2DData* SceneRenderer::GetFinalRT() {
            return Sentinel::FramebufferAPI::GetRenderTexture(m_FBuffer, 0);
        }

        Sentinel::UInt16 SceneRenderer::GetFinalRTWidth() {
            return Sentinel::FramebufferAPI::GetWidth(m_FBuffer);
        }

        Sentinel::UInt16 SceneRenderer::GetFinalRTHeight() {
            return Sentinel::FramebufferAPI::GetHeight(m_FBuffer);
        }

        void SceneRenderer::OnAttach() {
            Sentinel::ConstantbufferAPI::VSBind(m_CamCBuffer);

            Sentinel::Vector<std::pair<glm::vec4, glm::vec2>> vertices = {
                {{-3.0f, 3.0f, -7.0f, 1.0f}, {0.0f, 0.0f}},
                {{-1.0f, 3.0f, -7.0f, 1.0f}, {1.0f, 0.0f}},
                {{-1.0f, 1.0f, -7.0f, 1.0f}, {1.0f, 1.0f}},
                {{-3.0f, 1.0f, -7.0f, 1.0f}, {0.0f, 1.0f}},

                {{1.0f, 1.0f, -5.0f, 1.0f}, {0.0f, 0.0f}},
                {{3.0f, 1.0f, -5.0f, 1.0f}, {1.0f, 0.0f}},
                {{3.0f, -1.0f, -5.0f, 1.0f}, {1.0f, 1.0f}},
                {{1.0f, -1.0f, -5.0f, 1.0f}, {0.0f, 1.0f}}};

            Sentinel::Vector<Sentinel::UInt32> indices = {0, 1, 2, 0, 2, 3, 4, 5, 6, 4, 6, 7};

            m_VLayout = Sentinel::VertexbufferLayoutAPI::CreateVertexbufferLayoutData(m_VLayoutAlloc, m_Context);

            m_VBuffer = Sentinel::VertexbufferAPI::CreateVertexbufferData(
                m_VBufferAlloc, m_Context, vertices.Data(), vertices.Size() * sizeof(std::pair<glm::vec4, glm::vec2>));

            m_IBuffer = Sentinel::IndexbufferAPI::CreateIndexbufferData(
                m_IBufferAlloc, m_Context, indices.Data(), indices.Size());

            m_Shader = Sentinel::ShaderAPI::CreateShaderData(
                m_ShaderAlloc, m_Context, "../Engine/Resources/Shaders/TextureShader.hlsl", "TexShader");

            Sentinel::VertexbufferLayoutAPI::CreateLayout(m_VLayout, m_Shader);

            Sentinel::Texture2DDataImportSettings settings;
            settings.TextureFilepath = "../Engine/Resources/Images/Icon/512.png";
            m_TestTexture = Sentinel::Texture2DAPI::CreateTexture2DData(m_TexAlloc, m_Context, settings);

            m_Viewport = Sentinel::ViewportAPI::CreateViewportData(
                m_VPortAlloc, m_Context, 0, 0, m_Window->GetWidth(), m_Window->GetHeight(), 0, 1);

            m_FBuffer = Sentinel::FramebufferAPI::CreateFramebufferData(m_FBufferAlloc, m_Context, 1280, 720);
            Sentinel::FramebufferAPI::SetAttachments(m_FBuffer, {Sentinel::ColorFormat::RGBA8UNORM});

            Sentinel::VertexbufferLayoutAPI::Bind(m_VLayout);
            Sentinel::VertexbufferAPI::Bind(m_VBuffer, Sentinel::VertexbufferLayoutAPI::GetStride(m_VLayout));
            Sentinel::IndexbufferAPI::Bind(m_IBuffer);
            Sentinel::ShaderAPI::Bind(m_Shader);
            Sentinel::Texture2DAPI::Bind(m_TestTexture, 0, Sentinel::ShaderType::PIXEL);

            // Sentinel::SwapchainAPI::SetBuffers(m_Swapchain, m_RenderTexture, m_DepthTexture);

            Sentinel::FramebufferAPI::CreateAttachments(m_FBuffer, m_RTAlloc, m_DTAlloc);

            Resize(m_Window->GetWidth(), m_Window->GetHeight());
        }

        void SceneRenderer::OnDetach() {
            Sentinel::VertexbufferAPI::Unbind(m_VBuffer);
            Sentinel::IndexbufferAPI::Unbind(m_IBuffer);
            Sentinel::VertexbufferLayoutAPI::Unbind(m_VLayout);
            Sentinel::ShaderAPI::Unbind(m_Shader);
            Sentinel::Texture2DAPI::Unbind(m_TestTexture);
            // Sentinel::SwapchainAPI::UnsetBuffers(m_Swapchain);

            // m_Camera->Clean();
            Sentinel::ConstantbufferAPI::Clean(m_CamCBuffer);

            Sentinel::VertexbufferAPI::Clean(m_VBuffer);
            Sentinel::IndexbufferAPI::Clean(m_IBuffer);
            Sentinel::VertexbufferLayoutAPI::Clean(m_VLayout);

            Sentinel::Texture2DAPI::Clean(m_TestTexture);
            Sentinel::ShaderAPI::Clean(m_Shader);

            // Sentinel::SwapchainAPI::Clean(m_Swapchain);
            // Sentinel::ContextAPI::Clean(m_Context);

            m_VBufferAlloc.DeleteAll();
            m_IBufferAlloc.DeleteAll();
            m_VLayoutAlloc.DeleteAll();
            m_CBufferAlloc.DeleteAll();

            m_TexAlloc.DeleteAll();
            m_ShaderAlloc.DeleteAll();

            m_RTAlloc.DeleteAll();
            m_DTAlloc.DeleteAll();

            m_VPortAlloc.DeleteAll();

            m_VBufferAlloc.Deallocate();
            m_IBufferAlloc.Deallocate();
            m_VLayoutAlloc.Deallocate();
            m_CBufferAlloc.Deallocate();

            m_TexAlloc.Deallocate();
            m_ShaderAlloc.Deallocate();

            m_RTAlloc.Deallocate();
            m_DTAlloc.Deallocate();

            m_VPortAlloc.Deallocate();
        }

        void SceneRenderer::OnUpdate() {
            m_Camera->OnUpdate();
            m_ViewProj = m_Camera->GetViewProjectionMatrix();
            Sentinel::ConstantbufferAPI::SetDynamicData(m_CamCBuffer, &m_ViewProj);
        }

        void SceneRenderer::OnRender() {
            Sentinel::FramebufferAPI::Bind(m_FBuffer);
            Sentinel::ContextAPI::DrawIndexed(m_Context, Sentinel::IndexbufferAPI::GetCount(m_IBuffer));
        }

        void SceneRenderer::OnPostRender() {
            Sentinel::FramebufferAPI::Clear(m_FBuffer, {0.1f, 0.5f, 0.1f, 1.0f});
            Sentinel::FramebufferAPI::Unbind(m_FBuffer);
        }

        void SceneRenderer::Resize(Sentinel::UInt16 width, Sentinel::UInt16 height) {
            m_Camera->OnResize(width, height);
            // Sentinel::SwapchainAPI::Unbind(m_Swapchain);
            // Sentinel::SwapchainAPI::Resize(m_Swapchain, width, height);
            Sentinel::ViewportAPI::Resize(m_Viewport, width, height);
            Sentinel::ViewportAPI::Bind(m_Viewport);
            Sentinel::FramebufferAPI::Resize(m_FBuffer, width, height);
        }
    }  // namespace Rendering
}  // namespace Scribe