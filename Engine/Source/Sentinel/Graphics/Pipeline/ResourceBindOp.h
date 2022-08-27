#pragma once

#include "Sentinel/Common/Common.h"
#include "Sentinel/Graphics/Pipeline/IPipelineOperation.h"

namespace Sentinel {
    struct SwapchainData;

    struct VertexbufferData;
    struct IndexbufferData;
    struct VertexbufferLayoutData;

    struct ShaderData;
    struct Texture2DData;

    struct RenderTexture2DData;
    struct DepthTexture2DData;

    class ResourceBindOp final: public IPipelineOperation {
    public:
        ResourceBindOp(
            SwapchainData*& swapchain,
            VertexbufferData*& vBuffer,
            IndexbufferData*& iBuffer,
            VertexbufferLayoutData*& vLayout,
            ShaderData*& shader,
            Texture2DData*& texture,
            RenderTexture2DData*& renderTex,
            DepthTexture2DData*& depthTex);

    private:
        void Setup();
        void Execute();
        void Reset();

    private:
        SwapchainData*& m_Swapchain;
        VertexbufferData*& m_VBuffer;
        IndexbufferData*& m_IBuffer;
        VertexbufferLayoutData*& m_VLayout;
        ShaderData*& m_Shader;
        Texture2DData*& m_Texture;
        RenderTexture2DData*& m_RenderTexture;
        DepthTexture2DData*& m_DepthTexture;
    };
}  // namespace Sentinel