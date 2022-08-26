#pragma once

#include "Sentinel/Common/Common.h"
#include "Sentinel/Graphics/Pipeline/RenderPipeline.h"

namespace Sentinel {
    class Backend;
    class GraphicsMemoryManager;
    class Camera;

    struct ContextData;
    struct SwapchainData;
    struct FramebufferData;

    struct VertexbufferData;
    struct IndexbufferData;
    struct VertexbufferLayoutData;

    struct ShaderData;
    struct Texture2DData;

    struct RenderTexture2DData;
    struct DepthTexture2DData;


    class TestRenderer {
    public:
        TestRenderer();
        ~TestRenderer();

        void ConstructOperationQueue();

        void Setup();
        void Draw();
        void Unbind();

    private:
        RenderPipeline m_RenderPipeline;

        SharedRef<GraphicsMemoryManager> m_GFXMemory;
        SharedRef<Camera> m_Camera;
        // The objects to which these pointers are pointing to will be residing in GraphicsMemoryManager class, but it's
        // main pointers should be in the outer Renderer class
        ContextData* m_Context;
        SwapchainData* m_Swapchain;
        FramebufferData* m_Framebuffer;

        VertexbufferData* m_VBuffer;
        IndexbufferData* m_IBuffer;
        VertexbufferLayoutData* m_VLayout;

        ShaderData* m_Shader;
        Texture2DData* m_Texture;

        RenderTexture2DData* m_RenderTexture;
        DepthTexture2DData* m_DepthTexture;
    };
}  // namespace Sentinel