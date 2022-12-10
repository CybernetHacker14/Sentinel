#include "stpch.h"
#include "Sentinel/Graphics/Renderer/TestRenderer.h"

#include "Sentinel/Application/Application.h"
#include "Sentinel/Window/Window.h"

#include "Sentinel/Graphics/Common/Backend.h"
#include "Sentinel/Graphics/Common/GraphicsMemoryManager.h"
#include "Sentinel/Graphics/Camera/Camera.h"

#include "Sentinel/Graphics/Device/ContextAPI.h"
#include "Sentinel/Graphics/Device/SwapchainAPI.h"
#include "Sentinel/Graphics/Output/FramebufferAPI.h"
#include "Sentinel/Graphics/Output/ViewportAPI.h"

#include "Sentinel/Graphics/Buffer/VertexbufferAPI.h"
#include "Sentinel/Graphics/Buffer/IndexbufferAPI.h"
#include "Sentinel/Graphics/Buffer/VertexbufferLayoutAPI.h"

#include "Sentinel/Graphics/Material/ShaderAPI.h"

#include "Sentinel/Graphics/Texture/Texture2DAPI.h"
#include "Sentinel/Graphics/Texture/RenderTexture2DAPI.h"
#include "Sentinel/Graphics/Texture/DepthTexture2DAPI.h"

#include "Sentinel/GUI/ImGui/ImGuiLayer.h"

namespace Sentinel {

    Bool renderdocBuild = false;

    TestRenderer::TestRenderer() {
        Window& window = Application::Get().GetWindow();
        GLFWwindow* glfwWindow = window.GetNativeWindow<GLFWwindow>();
        m_GFXMemory = CreateSharedRef<GraphicsMemoryManager>();
        m_Context = ContextAPI::CreateImmediateContext(m_GFXMemory->ContextAllocator, glfwWindow);
        m_Swapchain = SwapchainAPI::CreateSwapchain(m_GFXMemory->SwapchainAllocator, m_Context, glfwWindow);
        m_Camera = CreateSharedRef<Camera>(m_GFXMemory, m_Context, window.GetWidth(), window.GetHeight());

        m_ImGuiLayer = new ImGuiLayer(m_Context);
    }

    TestRenderer::~TestRenderer() {
        // m_GFXMemory->ReleaseRef();
        delete (m_ImGuiLayer);
        ContextAPI::Clean(m_Context);
    }

    void TestRenderer::Construct() {
        STL::vector<STL::pair<glm::vec4, glm::vec2>> vertices = {
            {{-3.0f, 3.0f, -7.0f, 1.0f}, {0.0f, 0.0f}},
            {{-1.0f, 3.0f, -7.0f, 1.0f}, {1.0f, 0.0f}},
            {{-1.0f, 1.0f, -7.0f, 1.0f}, {1.0f, 1.0f}},
            {{-3.0f, 1.0f, -7.0f, 1.0f}, {0.0f, 1.0f}},

            {{1.0f, 1.0f, -5.0f, 1.0f}, {0.0f, 0.0f}},
            {{3.0f, 1.0f, -5.0f, 1.0f}, {1.0f, 0.0f}},
            {{3.0f, -1.0f, -5.0f, 1.0f}, {1.0f, 1.0f}},
            {{1.0f, -1.0f, -5.0f, 1.0f}, {0.0f, 1.0f}}};

        STL::vector<UInt32> indices = {0, 1, 2, 0, 2, 3, 4, 5, 6, 4, 6, 7};

        m_VLayout = VertexbufferLayoutAPI::CreateVertexbufferLayoutData(m_GFXMemory, m_Context);

        m_VBuffer = VertexbufferAPI::CreateVertexbufferData(
            m_GFXMemory, m_Context, vertices.data(), vertices.size() * sizeof(STL::pair<glm::vec4, glm::vec2>));

        m_IBuffer = IndexbufferAPI::CreateIndexbufferData(m_GFXMemory, m_Context, indices.data(), indices.size());

        if (renderdocBuild) {
            m_Shader = ShaderAPI::CreateShaderData(m_GFXMemory, m_Context, "TextureShader.hlsl", "TexShader");

        } else {
            m_Shader = ShaderAPI::CreateShaderData(
                m_GFXMemory, m_Context, "../Engine/Resources/Shaders/TextureShader.hlsl", "TexShader");
        }

        VertexbufferLayoutAPI::CreateLayout(m_VLayout, m_Shader);

        Texture2DDataImportSettings settings;
        if (renderdocBuild) {
            settings.TextureFilepath = "512.png";
        } else {
            settings.TextureFilepath = "../Engine/Resources/Images/Icon/512.png";
        }

        // TODO: Possible refactor
        Window& window = Application::Get().GetWindow();
        // \TODO

        m_RenderTexture = RenderTexture2DAPI::CreateRenderTexture2DData(
            m_GFXMemory->RenderTexture2DAllocator, m_Context, m_Swapchain);

        m_DepthTexture = DepthTexture2DAPI::CreateDepthTexture2DData(
            m_GFXMemory->DepthTexture2DAllocator,
            m_Context,
            window.GetWidth(),
            window.GetHeight(),
            DepthFormat::D24S8UINT,
            true);

        SwapchainAPI::SetBuffers(m_Swapchain, m_RenderTexture, m_DepthTexture);

        m_Viewport =
            ViewportAPI::CreateViewportData(m_GFXMemory, m_Context, 0, 0, window.GetWidth(), window.GetHeight(), 0, 1);
    }

    void TestRenderer::Setup() {
        VertexbufferLayoutAPI::Bind(m_VLayout);
        VertexbufferAPI::Bind(m_VBuffer, VertexbufferLayoutAPI::GetStride(m_VLayout));
        IndexbufferAPI::Bind(m_IBuffer);
        ShaderAPI::Bind(m_Shader);
        Texture2DAPI::Bind(m_Texture, 0, ShaderType::PIXEL);
        RenderTexture2DAPI::Bind(m_RenderTexture, 1, ShaderType::PIXEL);
        DepthTexture2DAPI::Bind(m_DepthTexture, 2, ShaderType::PIXEL);
        ViewportAPI::Bind(m_Viewport);
        Resize(Application::Get().GetWindow().GetWidth(), Application::Get().GetWindow().GetHeight());
    }

    void TestRenderer::Draw() {
        m_Camera->OnUpdate();
        SwapchainAPI::Bind(m_Swapchain);
        ContextAPI::DrawIndexed(m_Context, IndexbufferAPI::GetCount(m_IBuffer));
        SwapchainAPI::SwapBuffers(m_Swapchain);
        RenderTexture2DAPI::Clear(m_RenderTexture, {0.1f, 0.8f, 0.1f, 1.0f});
        DepthTexture2DAPI::Clear(m_DepthTexture);
        // ImGui rendering will happen here
        SwapchainAPI::Unbind(m_Swapchain);
    }

    void TestRenderer::Unbind() {
        SwapchainAPI::Unbind(m_Swapchain);
        VertexbufferAPI::Unbind(m_VBuffer);
        IndexbufferAPI::Unbind(m_IBuffer);
        VertexbufferLayoutAPI::Unbind(m_VLayout);
        ShaderAPI::Unbind(m_Shader);
        Texture2DAPI::Unbind(m_Texture);
        SwapchainAPI::UnsetBuffers(m_Swapchain);
        RenderTexture2DAPI::Unbind(m_RenderTexture);
        DepthTexture2DAPI::Unbind(m_DepthTexture);
    }

    void TestRenderer::Resize(UInt16 width, UInt16 height) {
        RenderTexture2DAPI::Unbind(m_RenderTexture);
        DepthTexture2DAPI::Unbind(m_DepthTexture);
        RenderTexture2DAPI::Clean(m_RenderTexture);
        DepthTexture2DAPI::Clean(m_DepthTexture);
        SwapchainAPI::Resize(m_Swapchain, width, height);
        RenderTexture2DAPI::Resize(m_RenderTexture, width, height);
        DepthTexture2DAPI::Resize(m_DepthTexture, width, height);
        ViewportAPI::Resize(m_Viewport, width, height);
        ViewportAPI::Bind(m_Viewport);
        RenderTexture2DAPI::Bind(m_RenderTexture, 1, ShaderType::PIXEL);
        DepthTexture2DAPI::Bind(m_DepthTexture, 2, ShaderType::PIXEL);
    }

    const UInt32 TestRenderer::GetTotalAllocations() {
        return m_GFXMemory->GetTotalAllocations();
    }

    const UInt32 TestRenderer::GetTotalFreeCount() {
        return m_GFXMemory->GetTotalFreeCount();
    }
}  // namespace Sentinel