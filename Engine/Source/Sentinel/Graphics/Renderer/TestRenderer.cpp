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

#include "Sentinel/Graphics/Buffer/VertexbufferAPI.h"
#include "Sentinel/Graphics/Buffer/IndexbufferAPI.h"
#include "Sentinel/Graphics/Buffer/VertexbufferLayoutAPI.h"

#include "Sentinel/Graphics/Material/ShaderAPI.h"

#include "Sentinel/Graphics/Texture/Texture2DAPI.h"
#include "Sentinel/Graphics/Texture/RenderTexture2DAPI.h"
#include "Sentinel/Graphics/Texture/DepthTexture2DAPI.h"

#include "Sentinel/GUI/ImGui/ImGuiLayer.h"
#include "Sentinel/GUI/ImGui/ImGuiDebugLayer.h"

namespace Sentinel {
    TestRenderer::TestRenderer() {
        Window& window = Application::Get().GetWindow();
        GLFWwindow* glfwWindow = window.GetNativeWindow<GLFWwindow>();
        m_GFXMemory = CreateSharedRef<GraphicsMemoryManager>();
        m_Context = ContextAPI::CreateImmediateContext(m_GFXMemory, glfwWindow);
        m_Swapchain = SwapchainAPI::CreateSwapchain(m_GFXMemory, m_Context, glfwWindow);
        // m_Framebuffer =
        // FramebufferAPI::CreateFramebufferData(m_GFXMemory, m_Context, window.GetWidth(), window.GetHeight());
        m_Camera = CreateSharedRef<Camera>(m_GFXMemory, m_Context, window.GetWidth(), window.GetHeight());

        /*m_ImGuiLayer = new ImGuiLayer(m_Context);
        Application::Get().PushOverlay(m_ImGuiLayer);
        m_ImGuiDebugLayer = new ImGuiDebugLayer(m_Camera);
        Application::Get().PushOverlay(m_ImGuiDebugLayer);*/
    }

    TestRenderer::~TestRenderer() {
        // m_GFXMemory->ReleaseRef();
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

        m_Shader = ShaderAPI::CreateShaderData(
            m_GFXMemory, m_Context, "../Engine/Resources/Shaders/TextureShader.hlsl", "TexShader");

        // m_Shader = ShaderAPI::CreateShaderData(m_GFXMemory, m_Context, "TextureShader.hlsl", "TexShader");

        VertexbufferLayoutAPI::CreateLayout(m_VLayout, m_Shader);

        Texture2DDataImportSettings settings;
        settings.TextureFilepath = "Assets/Tile1.jpg";

        m_Texture = Texture2DAPI::CreateTexture2DData(m_GFXMemory, m_Context, settings);

        // TODO: Possible refactor
        Window& window = Application::Get().GetWindow();
        // \TODO

        m_RenderTexture = RenderTexture2DAPI::CreateRenderTexture2DData(m_GFXMemory, m_Context, m_Swapchain);

        m_DepthTexture = DepthTexture2DAPI::CreateDepthTexture2DData(
            m_GFXMemory, m_Context, window.GetWidth(), window.GetHeight(), DepthFormat::D24S8UINT, true);

        SwapchainAPI::SetBuffers(m_Swapchain, m_RenderTexture, m_DepthTexture);
    }

    void TestRenderer::Setup() {
        VertexbufferLayoutAPI::Bind(m_VLayout);
        UInt32 value = VertexbufferLayoutAPI::GetStride(m_VLayout);
        VertexbufferAPI::Bind(m_VBuffer, VertexbufferLayoutAPI::GetStride(m_VLayout));
        IndexbufferAPI::Bind(m_IBuffer);
        ShaderAPI::Bind(m_Shader);
        Texture2DAPI::Bind(m_Texture, 0, ShaderType::PIXEL);
        RenderTexture2DAPI::Bind(m_RenderTexture, 1, ShaderType::PIXEL);
        DepthTexture2DAPI::Bind(m_DepthTexture, 2, ShaderType::PIXEL);
        SwapchainAPI::Bind(m_Swapchain);
    }

    void TestRenderer::Draw() {
        RenderTexture2DAPI::Clear(m_RenderTexture, {0.1f, 0.8f, 0.1f, 1.0f});
        ContextAPI::DrawIndexed(m_Context, IndexbufferAPI::GetCount(m_IBuffer));
        SwapchainAPI::SwapBuffers(m_Swapchain);
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

    const UInt32 TestRenderer::GetTotalAllocations() {
        return m_GFXMemory->GetTotalAllocations();
    }

    const UInt32 TestRenderer::GetTotalFreeCount() {
        return m_GFXMemory->GetTotalFreeCount();
    }
}  // namespace Sentinel