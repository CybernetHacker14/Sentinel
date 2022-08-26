#pragma once

#include "Sentinel/Common/Common.h"
#include "Sentinel/Graphics/Pipeline/IPipelineOperation.h"

namespace Sentinel {
    class GraphicsMemoryManager;

    struct ContextData;
    struct SwapchainData;

    struct VertexbufferData;
    struct IndexbufferData;
    struct VertexbufferLayoutData;

    struct ShaderData;
    struct Texture2DData;

    struct RenderTexture2DData;
    struct DepthTexture2DData;

    class Camera;

    class ResourceCreateOp final: public IPipelineOperation {
    public:
        ResourceCreateOp(
            SharedRef<GraphicsMemoryManager> memoryHandle, ContextData*& context, SwapchainData*& swapchain);

        inline void SetGFXComponentPointers(
            VertexbufferData** vBuffer,
            IndexbufferData** iBuffer,
            VertexbufferLayoutData** vLayout,
            ShaderData** shader,
            Texture2DData** texture,
            RenderTexture2DData** renderTex,
            DepthTexture2DData** depthTex) {
            m_VBuffer = vBuffer;
            m_IBuffer = iBuffer;
            m_VLayout = vLayout;
            m_Shader = shader;
            m_Texture = texture;
            m_RenderTexture = renderTex;
            m_DepthTexture = depthTex;
        }

    private:
        void Setup();
        void Execute();
        void Reset();

    private:
        SharedRef<GraphicsMemoryManager> m_MemoryHandle;
        ContextData*& m_Context;
        SwapchainData*& m_Swapchain;
        VertexbufferData** m_VBuffer;
        IndexbufferData** m_IBuffer;
        VertexbufferLayoutData** m_VLayout;
        ShaderData** m_Shader;
        Texture2DData** m_Texture;
        RenderTexture2DData** m_RenderTexture;
        DepthTexture2DData** m_DepthTexture;
    };
}  // namespace Sentinel