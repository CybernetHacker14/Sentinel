#pragma once

#include "Sentinel/Common/Common.h"

namespace Sentinel {
    class Backend;
    class GraphicsMemoryManager;
    class Camera;

    struct ContextData;
    struct SwapchainData;
    struct FramebufferData;
    struct ViewportData;

    struct VertexbufferData;
    struct IndexbufferData;
    struct VertexbufferLayoutData;

    struct ShaderData;
    struct Texture2DData;

    struct RenderTexture2DData;
    struct DepthTexture2DData;

    class ImGuiLayer;

    class TestRenderer: public ISharedRef {
    public:
        TestRenderer();
        ~TestRenderer();

        void Construct();

        void Setup();
        void Draw();
        void Unbind();

        void Resize(UInt16 width, UInt16 height);

        SharedRef<Camera> GetCamera() { return m_Camera; }

        const UInt32 GetTotalAllocations();
        const UInt32 GetTotalFreeCount();

    private:
        SharedRef<GraphicsMemoryManager> m_GFXMemory;
        SharedRef<Camera> m_Camera;
        // The objects to which these pointers are pointing to will be residing in GraphicsMemoryManager class, but it's
        // main pointers should be in the outer Renderer class
        ContextData* m_Context;
        SwapchainData* m_Swapchain;
        FramebufferData* m_Framebuffer;
        ViewportData* m_Viewport;

        VertexbufferData* m_VBuffer;
        IndexbufferData* m_IBuffer;
        VertexbufferLayoutData* m_VLayout;

        ShaderData* m_Shader;
        Texture2DData* m_Texture;

        RenderTexture2DData* m_RenderTexture;
        DepthTexture2DData* m_DepthTexture;

        // Add ImGuiLayer and ImGuiDebugLayer here
        ImGuiLayer* m_ImGuiLayer;

        Texture2DData* m_IconTexture;
    };
}  // namespace Sentinel