#pragma once

#include "Sentinel/Common/Common.h"
#include "Sentinel/Memory/PoolAllocator.h"
#include "Sentinel/Graphics/Device/SwapchainData.h"

struct GLFWwindow;

namespace Sentinel {
    class SwapchainAPI {
    public:
        static SwapchainData* CreateSwapchain(
            PoolAllocator<SwapchainData>& allocator, ContextData* context, GLFWwindow* windowHandle);

        static void SwapBuffers(SwapchainData* dataObject);

        static void Resize(SwapchainData* dataObject, UInt16 width, UInt16 height);

        static void Bind(SwapchainData* dataObject);

        static void Unbind(SwapchainData* dataObject);

        // Relaying the creation and deletion to external components, cause creating render textures and depth textures
        // requires a lot of parameters in place, which breaks the API modularity.
        // So as of now, Swapchain backbuffer creation is EXPLICIT.
        static void SetBuffers(
            SwapchainData* dataObject, RenderTexture2DData* renderTexture, DepthTexture2DData* depthTexture);

        // Since the creation is external, maybe the deletion can be external as well, and we just null the buffer
        // pointers here
        static void UnsetBuffers(SwapchainData* dataObject);

#ifdef ST_RENDERER_DX11
        inline static IDXGISwapChain* GetNativeSwapchain(SwapchainData* databject) { return databject->m_Swapchain; }
#endif  // ST_RENDERER_DX11

    private:
        static void Init(SwapchainData* dataObject, GLFWwindow* windowHandle);
    };
}  // namespace Sentinel