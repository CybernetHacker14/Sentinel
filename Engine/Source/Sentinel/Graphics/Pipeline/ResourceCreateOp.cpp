#include "stpch.h"
#include "Sentinel/Graphics/Pipeline/ResourceCreateOp.h"

#include "Sentinel/Application/Application.h"
#include "Sentinel/Graphics/Common/GraphicsMemoryManager.h"
#include "Sentinel/Graphics/Device/ContextAPI.h"
#include "Sentinel/Graphics/Device/SwapchainAPI.h"

#include "Sentinel/Graphics/Buffer/VertexbufferAPI.h"
#include "Sentinel/Graphics/Buffer/IndexbufferAPI.h"
#include "Sentinel/Graphics/Buffer/VertexbufferLayoutAPI.h"

#include "Sentinel/Graphics/Material/ShaderAPI.h"

#include "Sentinel/Graphics/Texture/Texture2DAPI.h"
#include "Sentinel/Graphics/Texture/RenderTexture2DAPI.h"
#include "Sentinel/Graphics/Texture/DepthTexture2DAPI.h"

#include <glm/glm.hpp>

namespace Sentinel {
    ResourceCreateOp::ResourceCreateOp(
        SharedRef<GraphicsMemoryManager> memoryHandle, ContextData*& context, SwapchainData*& swapchain)
        : m_MemoryHandle(memoryHandle), m_Context(context), m_Swapchain(swapchain) {
        m_SetupFn = ST_BIND_EVENT_FN(ResourceCreateOp::Setup);
        m_ExecuteFn = ST_BIND_EVENT_FN(ResourceCreateOp::Execute);
        m_ResetFn = ST_BIND_EVENT_FN(ResourceCreateOp::Reset);
    }

    void ResourceCreateOp::Setup() {
    }

    void ResourceCreateOp::Execute() {
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

        (*m_VBuffer) = VertexbufferAPI::CreateVertexbufferData(
            m_MemoryHandle, m_Context, vertices.data(), vertices.size() * sizeof(STL::pair<glm::vec4, glm::vec2>));

        (*m_IBuffer) = IndexbufferAPI::CreateIndexbufferData(m_MemoryHandle, m_Context, indices.data(), indices.size());

        (*m_VLayout) = VertexbufferLayoutAPI::CreateVertexbufferLayoutData(m_MemoryHandle, m_Context);

        (*m_Shader) = ShaderAPI::CreateShaderData(
            m_MemoryHandle, m_Context, "../Engine/Resources/Shaders/TextureShader.hlsl", "TexShader");

        Texture2DDataImportSettings settings;
        settings.TextureFilepath = "Assets/Tile1.jpg";

        (*m_Texture) = Texture2DAPI::CreateTexture2DData(m_MemoryHandle, m_Context, settings);

        // TODO: Possible refactor
        Window& window = Application::Get().GetWindow();
        // \TODO

        (*m_RenderTexture) = RenderTexture2DAPI::CreateRenderTexture2DData(
            m_MemoryHandle, m_Context, window.GetWidth(), window.GetHeight(), ColorFormat::RGBA32F, true);

        (*m_DepthTexture) = DepthTexture2DAPI::CreateDepthTexture2DData(
            m_MemoryHandle, m_Context, window.GetWidth(), window.GetHeight(), DepthFormat::D24S8UINT, true);
    }

    void ResourceCreateOp::Reset() {
    }
}  // namespace Sentinel