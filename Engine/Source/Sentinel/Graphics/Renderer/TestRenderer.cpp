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

#include "Sentinel/Graphics/Pipeline/RenderPipeline.h"

#include "Sentinel/Graphics/Pipeline/ResourceCreateOp.h"
#include "Sentinel/Graphics/Pipeline/ResourceBindOp.h"
#include "Sentinel/Graphics/Pipeline/ResourceRenderOp.h"
#include "Sentinel/Graphics/Pipeline/ResourceUnbindOp.h"

namespace Sentinel {
    TestRenderer::TestRenderer() {
        Window& window = Application::Get().GetWindow();
        GLFWwindow* glfwWindow = window.GetNativeWindow<GLFWwindow>();
        m_GFXMemory = CreateSharedRef<GraphicsMemoryManager>();
        m_Context = ContextAPI::CreateImmediateContext(m_GFXMemory, glfwWindow);
        m_Swapchain = SwapchainAPI::CreateSwapchain(m_GFXMemory, m_Context, glfwWindow);
        m_Framebuffer =
            FramebufferAPI::CreateFramebufferData(m_GFXMemory, m_Context, window.GetWidth(), window.GetHeight());
        m_Camera = CreateSharedRef<Camera>(m_GFXMemory, m_Context, window.GetWidth(), window.GetHeight());
    }

    TestRenderer::~TestRenderer() {
    }

    void TestRenderer::ConstructOperationQueue() {
        ResourceCreateOp createOp(m_GFXMemory, m_Context, m_Swapchain);
        createOp.SetGFXComponentPointers(
            &m_VBuffer, &m_IBuffer, &m_VLayout, &m_Shader, &m_Texture, &m_RenderTexture, &m_DepthTexture);
        ResourceBindOp bindOp(m_Swapchain);
        bindOp.SetGFXComponentPointers(
            &m_VBuffer, &m_IBuffer, &m_VLayout, &m_Shader, &m_Texture, &m_RenderTexture, &m_DepthTexture);

        ResourceRenderOp renderOp(m_Context, m_Swapchain);
        renderOp.SetGFXComponentPointers(&m_IBuffer, &m_RenderTexture, &m_DepthTexture);

        ResourceUnbindOp unbindOP(m_Swapchain);

        m_RenderPipeline.m_CreateOpQueue.emplace_back(STL::move(createOp));
        m_RenderPipeline.m_CreateOpQueue.emplace_back(STL::move(bindOp));
        m_RenderPipeline.m_RenderOpQueue.emplace_back(STL::move(renderOp));
        m_RenderPipeline.m_UnbindOpQueue.emplace_back(STL::move(unbindOP));
    }

    void TestRenderer::Setup() {
        for (auto& operation: m_RenderPipeline.m_CreateOpQueue) {
            operation.Setup();
            operation.Execute();
            operation.Reset();
        }
    }

    void TestRenderer::Draw() {
        for (auto& operation: m_RenderPipeline.m_RenderOpQueue) {
            operation.Setup();
            operation.Execute();
            operation.Reset();
        }
    }

    void TestRenderer::Unbind() {
        for (auto& operation: m_RenderPipeline.m_UnbindOpQueue) {
            operation.Setup();
            operation.Execute();
            operation.Reset();
        }
    }
}  // namespace Sentinel