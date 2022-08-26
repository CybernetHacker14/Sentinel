#pragma once

#include "Sentinel/Common/Common.h"
#include "Sentinel/Graphics/Pipeline/IPipelineOperation.h"

namespace Sentinel {
    struct ContextData;
    struct SwapchainData;
    struct IndexbufferData;
    struct RenderTexture2DData;
    struct DepthTexture2DData;

    class ResourceRenderOp final: public IPipelineOperation {
    public:
        ResourceRenderOp(ContextData*& context, SwapchainData*& swapchain);

        inline void SetGFXComponentPointers(
            IndexbufferData** iBuffer, RenderTexture2DData** renderTex, DepthTexture2DData** depthTex) {
            m_IBuffer = iBuffer;
            m_RenderTexture = renderTex;
            m_DepthTexture = depthTex;
        }

    private:
        void Setup();
        void Execute();
        void Reset();

    private:
        ContextData*& m_Context;
        SwapchainData*& m_Swapchain;
        IndexbufferData** m_IBuffer;
        RenderTexture2DData** m_RenderTexture;
        DepthTexture2DData** m_DepthTexture;
    };
}  // namespace Sentinel