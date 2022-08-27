#include "stpch.h"
#include "Sentinel/Graphics/Pipeline/ResourceBindOp.h"

#include "Sentinel/Application/Application.h"
#include "Sentinel/Graphics/Device/SwapchainAPI.h"

#include "Sentinel/Graphics/Buffer/VertexbufferAPI.h"
#include "Sentinel/Graphics/Buffer/IndexbufferAPI.h"
#include "Sentinel/Graphics/Buffer/VertexbufferLayoutAPI.h"

#include "Sentinel/Graphics/Material/ShaderAPI.h"

#include "Sentinel/Graphics/Texture/Texture2DAPI.h"
#include "Sentinel/Graphics/Texture/RenderTexture2DAPI.h"
#include "Sentinel/Graphics/Texture/DepthTexture2DAPI.h"

namespace Sentinel {
    ResourceBindOp::ResourceBindOp(
        SwapchainData*& swapchain,
        VertexbufferData*& vBuffer,
        IndexbufferData*& iBuffer,
        VertexbufferLayoutData*& vLayout,
        ShaderData*& shader,
        Texture2DData*& texture,
        RenderTexture2DData*& renderTex,
        DepthTexture2DData*& depthTex)
        : m_Swapchain(swapchain),
          m_VBuffer(vBuffer),
          m_IBuffer(iBuffer),
          m_VLayout(vLayout),
          m_Shader(shader),
          m_Texture(texture),
          m_RenderTexture(renderTex),
          m_DepthTexture(depthTex) {
        m_SetupFn = ST_BIND_EVENT_FN(ResourceBindOp::Setup);
        m_ExecuteFn = ST_BIND_EVENT_FN(ResourceBindOp::Execute);
        m_ResetFn = ST_BIND_EVENT_FN(ResourceBindOp::Reset);
    }

    void ResourceBindOp::Setup() {
        SwapchainAPI::SetBuffers(m_Swapchain, m_RenderTexture, m_DepthTexture);
    }

    void ResourceBindOp::Execute() {
        VertexbufferLayoutAPI::Bind(m_VLayout);
        VertexbufferAPI::Bind(m_VBuffer, VertexbufferLayoutAPI::GetStride(m_VLayout));
        IndexbufferAPI::Bind(m_IBuffer);
        ShaderAPI::Bind(m_Shader);
        Texture2DAPI::Bind(m_Texture, 0, ShaderType::PIXEL);
        SwapchainAPI::Bind(m_Swapchain);
        // FramebufferAPI::Bind(m_Framebuffer);
        // RenderTexture2DAPI::Bind(*m_RenderTexture, 1, ShaderType::PIXEL);
        // DepthTexture2DAPI::Bind(*m_DepthTexture, 2, ShaderType::PIXEL);
    }

    void ResourceBindOp::Reset() {
    }
}  // namespace Sentinel