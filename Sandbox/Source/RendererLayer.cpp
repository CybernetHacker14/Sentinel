#include "RendererLayer.h"

#include <Sentinel/Graphics/Camera/Camera.h>
#include <Sentinel/Filesystem/Filesystem.h>
#include <Sentinel/Common/Containers/Vector.h>
#include <Sentinel/Window/Window.h>

#include <glm/glm.hpp>

namespace Sandbox {
    namespace Rendering {

        static Sentinel::Bool renderdocBuild = false;

        RendererLayer::RendererLayer(Sentinel::Window* window) : m_Window(window) {
            m_ResizeIndex = Sentinel::EventsAPI::RegisterEvent(
                Sentinel::EventType::WindowResize, this, ST_BIND_FN(RendererLayer::OnWindowResize));

            m_CtxAlloc.Allocate(1);
            m_SCAlloc.Allocate(1);
            m_VPortAlloc.Allocate(1);

            m_VBufferAlloc.Allocate(1);
            m_IBufferAlloc.Allocate(1);
            m_LayoutAlloc.Allocate(1);
            m_CBufferAlloc.Allocate(1);

            m_ShaderAlloc.Allocate(1);
            m_TexAlloc.Allocate(1);

            m_RTAlloc.Allocate(1);
            m_DTAlloc.Allocate(1);

            m_ImageLoader.Allocate(1);

            GLFWwindow* glfwWindow = static_cast<GLFWwindow*>(m_Window->GetNative());
            m_Context = Sentinel::ContextAPI::CreateImmediateContext(m_CtxAlloc, glfwWindow);
            m_Swapchain = Sentinel::SwapchainAPI::CreateSwapchain(m_SCAlloc, m_Context, glfwWindow);
            m_CamCBuffer = Sentinel::ConstantbufferAPI::CreateConstantbufferData(
                m_CBufferAlloc, m_Context, sizeof(glm::mat4), 0, Sentinel::CBufferUsageType::DYNAMIC);
            m_Camera = new Sentinel::Camera(m_Window->GetWidth(), m_Window->GetHeight());
        }

        RendererLayer::~RendererLayer() {
            delete m_Camera;

            m_VBufferAlloc.DeleteAll();
            m_IBufferAlloc.DeleteAll();
            m_LayoutAlloc.DeleteAll();
            m_CBufferAlloc.DeleteAll();

            m_TexAlloc.DeleteAll();
            m_ShaderAlloc.DeleteAll();

            m_RTAlloc.DeleteAll();
            m_DTAlloc.DeleteAll();

            m_VPortAlloc.DeleteAll();
            m_SCAlloc.DeleteAll();
            m_CtxAlloc.DeleteAll();

            m_ImageLoader.DeleteAll();

            m_VBufferAlloc.Deallocate();
            m_IBufferAlloc.Deallocate();
            m_LayoutAlloc.Deallocate();
            m_CBufferAlloc.Deallocate();

            m_TexAlloc.Deallocate();
            m_ShaderAlloc.Deallocate();

            m_RTAlloc.Deallocate();
            m_DTAlloc.Deallocate();

            m_VPortAlloc.Deallocate();
            m_SCAlloc.Deallocate();
            m_CtxAlloc.Deallocate();

            m_ImageLoader.Deallocate();
        }

        void RendererLayer::OnAttach() {
            Sentinel::ConstantbufferAPI::VSBind(m_CamCBuffer);

            std::vector<std::pair<glm::vec4, glm::vec2>> vertices = {
                {{-3.0f, 3.0f, -7.0f, 1.0f}, {0.0f, 0.0f}},
                {{-1.0f, 3.0f, -7.0f, 1.0f}, {1.0f, 0.0f}},
                {{-1.0f, 1.0f, -7.0f, 1.0f}, {1.0f, 1.0f}},
                {{-3.0f, 1.0f, -7.0f, 1.0f}, {0.0f, 1.0f}},

                {{1.0f, 1.0f, -5.0f, 1.0f}, {0.0f, 0.0f}},
                {{3.0f, 1.0f, -5.0f, 1.0f}, {1.0f, 0.0f}},
                {{3.0f, -1.0f, -5.0f, 1.0f}, {1.0f, 1.0f}},
                {{1.0f, -1.0f, -5.0f, 1.0f}, {0.0f, 1.0f}}};

            std::vector<Sentinel::UInt32> indices = {0, 1, 2, 0, 2, 3, 4, 5, 6, 4, 6, 7};

            m_VLayout = Sentinel::VertexbufferLayoutAPI::CreateVertexbufferLayoutData(m_LayoutAlloc, m_Context);

            m_VBuffer = Sentinel::VertexbufferAPI::CreateVertexbufferData(
                m_VBufferAlloc, m_Context, vertices.data(), vertices.size() * sizeof(std::pair<glm::vec4, glm::vec2>));

            m_IBuffer = Sentinel::IndexbufferAPI::CreateIndexbufferData(
                m_IBufferAlloc, m_Context, indices.data(), indices.size());

            if (!renderdocBuild) {
                m_Shader = Sentinel::ShaderAPI::CreateShaderData(
                    m_ShaderAlloc, m_Context, "../Engine/Resources/Shaders/TextureShader.hlsl", "TexShader");

            } else {
                m_Shader =
                    Sentinel::ShaderAPI::CreateShaderData(m_ShaderAlloc, m_Context, "TextureShader.hlsl", "TexShader");
            }

            Sentinel::VertexbufferLayoutAPI::CreateLayout(m_VLayout, m_Shader);

            /*if (!renderdocBuild)
                settings.TextureFilepath = "../Engine/Resources/Images/Icon/512.png";
            else
                settings.TextureFilepath = "512.png";

            m_Texture = Sentinel::Texture2DAPI::CreateTexture2DData(m_TexAlloc, m_Context, settings);*/

            Sentinel::ImageResource* resource;
            Sentinel::ImageResourceLoader::LoadFromFile("../Scribe/Grid.sibf", &resource);

            Sentinel::Texture2DDataImportSettings settings;
            m_Texture = Sentinel::Texture2DAPI::CreateTexture2DData(
                m_TexAlloc,
                m_Context,
                settings,
                resource->Pixels,
                resource->Width,
                resource->Height,
                resource->Channels);

            m_RenderTexture =
                Sentinel::RenderTexture2DAPI::CreateRenderTexture2DData(m_RTAlloc, m_Context, m_Swapchain);

            m_DepthTexture = Sentinel::DepthTexture2DAPI::CreateDepthTexture2DData(
                m_DTAlloc,
                m_Context,
                m_Window->GetWidth(),
                m_Window->GetHeight(),
                Sentinel::DepthFormat::D24S8UINT,
                true);

            m_Viewport = Sentinel::ViewportAPI::CreateViewportData(
                m_VPortAlloc, m_Context, 0, 0, m_Window->GetWidth(), m_Window->GetHeight(), 0, 1);

            Sentinel::ViewportAPI::Bind(m_Viewport);

            Sentinel::VertexbufferLayoutAPI::Bind(m_VLayout);
            Sentinel::VertexbufferAPI::Bind(m_VBuffer, Sentinel::VertexbufferLayoutAPI::GetStride(m_VLayout));
            Sentinel::IndexbufferAPI::Bind(m_IBuffer);
            Sentinel::ShaderAPI::Bind(m_Shader);
            Sentinel::Texture2DAPI::Bind(m_Texture, 0, Sentinel::ShaderType::PIXEL);

            Sentinel::RenderTexture2DAPI::Bind(m_RenderTexture, 1, Sentinel::ShaderType::PIXEL);
            Sentinel::DepthTexture2DAPI::Bind(m_DepthTexture, 2, Sentinel::ShaderType::PIXEL);

            Sentinel::SwapchainAPI::SetBuffers(m_Swapchain, m_RenderTexture, m_DepthTexture);

            Resize(m_Window->GetWidth(), m_Window->GetHeight());
        }

        void RendererLayer::OnDetach() {
            Sentinel::SwapchainAPI::Unbind(m_Swapchain);
            Sentinel::VertexbufferAPI::Unbind(m_VBuffer);
            Sentinel::IndexbufferAPI::Unbind(m_IBuffer);
            Sentinel::VertexbufferLayoutAPI::Unbind(m_VLayout);
            Sentinel::ShaderAPI::Unbind(m_Shader);
            Sentinel::Texture2DAPI::Unbind(m_Texture);
            Sentinel::SwapchainAPI::UnsetBuffers(m_Swapchain);
            Sentinel::RenderTexture2DAPI::Unbind(m_RenderTexture);
            Sentinel::DepthTexture2DAPI::Unbind(m_DepthTexture);

            // m_Camera->Clean();
            Sentinel::ConstantbufferAPI::Clean(m_CamCBuffer);

            Sentinel::VertexbufferAPI::Clean(m_VBuffer);
            Sentinel::IndexbufferAPI::Clean(m_IBuffer);
            Sentinel::VertexbufferLayoutAPI::Clean(m_VLayout);

            Sentinel::Texture2DAPI::Clean(m_Texture);
            Sentinel::ShaderAPI::Clean(m_Shader);

            Sentinel::RenderTexture2DAPI::Clean(m_RenderTexture);
            Sentinel::DepthTexture2DAPI::Clean(m_DepthTexture);

            Sentinel::SwapchainAPI::Clean(m_Swapchain);
            Sentinel::ContextAPI::Clean(m_Context);

            m_VBufferAlloc.DeleteAll();
            m_IBufferAlloc.DeleteAll();
            m_LayoutAlloc.DeleteAll();
            m_CBufferAlloc.DeleteAll();

            m_TexAlloc.DeleteAll();
            m_ShaderAlloc.DeleteAll();

            m_RTAlloc.DeleteAll();
            m_DTAlloc.DeleteAll();

            m_VPortAlloc.DeleteAll();
            m_SCAlloc.DeleteAll();
            m_CtxAlloc.DeleteAll();

            m_VBufferAlloc.Deallocate();
            m_IBufferAlloc.Deallocate();
            m_LayoutAlloc.Deallocate();
            m_CBufferAlloc.Deallocate();

            m_TexAlloc.Deallocate();
            m_ShaderAlloc.Deallocate();

            m_RTAlloc.Deallocate();
            m_DTAlloc.Deallocate();

            m_VPortAlloc.Deallocate();
            m_SCAlloc.Deallocate();
            m_CtxAlloc.Deallocate();
        }

        void RendererLayer::OnUpdate() {
            m_Camera->OnUpdate();
            m_ViewProj = m_Camera->GetViewProjectionMatrix();
            Sentinel::ConstantbufferAPI::SetDynamicData(m_CamCBuffer, &m_ViewProj);
        }

        void RendererLayer::OnRender() {
            Sentinel::SwapchainAPI::Bind(m_Swapchain);
            Sentinel::ContextAPI::DrawIndexed(m_Context, Sentinel::IndexbufferAPI::GetCount(m_IBuffer));
            Sentinel::SwapchainAPI::SwapBuffers(m_Swapchain);
            Sentinel::RenderTexture2DAPI::Clear(m_RenderTexture, {0.1f, 0.5f, 0.1f, 1.0f});
            Sentinel::DepthTexture2DAPI::Clear(m_DepthTexture);
        }

        void RendererLayer::OnImGuiRender() {
        }

        void RendererLayer::OnPostRender() {
            Sentinel::SwapchainAPI::Unbind(m_Swapchain);
        }

        Sentinel::Bool RendererLayer::OnWindowResize(
            Sentinel::EventType type, Sentinel::EventData data, void* listener) {
            RendererLayer* renderer = (RendererLayer*)listener;
            renderer->m_Camera->OnResize(data.UInt16[0], data.UInt16[1]);
            renderer->Resize(data.UInt16[0], data.UInt16[1]);
            return true;
        }

        void RendererLayer::Resize(Sentinel::UInt16 width, Sentinel::UInt16 height) {
            Sentinel::RenderTexture2DAPI::Unbind(m_RenderTexture);
            Sentinel::DepthTexture2DAPI::Unbind(m_DepthTexture);
            Sentinel::RenderTexture2DAPI::Clean(m_RenderTexture);
            Sentinel::DepthTexture2DAPI::Clean(m_DepthTexture);
            Sentinel::SwapchainAPI::Unbind(m_Swapchain);
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