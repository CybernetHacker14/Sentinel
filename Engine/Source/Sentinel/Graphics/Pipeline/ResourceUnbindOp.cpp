#include "stpch.h"
#include "Sentinel/Graphics/Pipeline/ResourceUnbindOp.h"

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
    ResourceUnbindOp::ResourceUnbindOp(SwapchainData*& swapchain) : m_Swapchain(swapchain) {
        m_SetupFn = ST_BIND_EVENT_FN(ResourceUnbindOp::Setup);
        m_ExecuteFn = ST_BIND_EVENT_FN(ResourceUnbindOp::Execute);
        m_ResetFn = ST_BIND_EVENT_FN(ResourceUnbindOp::Reset);
    }

    void ResourceUnbindOp::Setup() {
        SwapchainAPI::UnsetBuffers(m_Swapchain);
    }

    void ResourceUnbindOp::Execute() {
        VertexbufferAPI::Unbind(*m_VBuffer);
        IndexbufferAPI::Unbind(*m_IBuffer);
        VertexbufferLayoutAPI::Unbind(*m_VLayout);
        ShaderAPI::Unbind(*m_Shader);
        Texture2DAPI::Unbind(*m_Texture);
        SwapchainAPI::Unbind(m_Swapchain);
    }

    void ResourceUnbindOp::Reset() {
    }
}  // namespace Sentinel