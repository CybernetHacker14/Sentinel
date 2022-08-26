#include "stpch.h"
#include "Sentinel/Graphics/Pipeline/ResourceRenderOp.h"

#include "Sentinel/Application/Application.h"
#include "Sentinel/Graphics/Common/GraphicsMemoryManager.h"
#include "Sentinel/Graphics/Device/ContextAPI.h"
#include "Sentinel/Graphics/Device/SwapchainAPI.h"
#include "Sentinel/Graphics/Buffer/IndexbufferAPI.h"

#include "Sentinel/Graphics/Texture/RenderTexture2DAPI.h"
#include "Sentinel/Graphics/Texture/DepthTexture2DAPI.h"

namespace Sentinel {
    ResourceRenderOp::ResourceRenderOp(ContextData*& context, SwapchainData*& swapchain)
        : m_Context(context), m_Swapchain(swapchain) {
        m_SetupFn = ST_BIND_EVENT_FN(ResourceRenderOp::Setup);
        m_ExecuteFn = ST_BIND_EVENT_FN(ResourceRenderOp::Execute);
        m_ResetFn = ST_BIND_EVENT_FN(ResourceRenderOp::Reset);
    }

    void ResourceRenderOp::Setup() {
    }

    void ResourceRenderOp::Execute() {
        RenderTexture2DAPI::Clear(*m_RenderTexture, {0.1f, 0.1f, 0.1f, 1.0f});
        DepthTexture2DAPI::Clear(*m_DepthTexture);
        ContextAPI::DrawIndexed(m_Context, IndexbufferAPI::GetCount(*m_IBuffer));
        SwapchainAPI::SwapBuffers(m_Swapchain);
    }

    void ResourceRenderOp::Reset() {
    }
}  // namespace Sentinel