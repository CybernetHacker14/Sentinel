#pragma once

#include "Sentinel/Common/Core/DataTypes.h"

#ifdef ST_RENDERER_DX11
struct IDXGISwapChain;
#endif  // ST_RENDERER_DX11

namespace Sentinel {
    struct ContextData;
    struct RenderTexture2DData;
    struct DepthTexture2DData;

    struct SwapchainData {
    public:
        ContextData* Context;

    private:
        Bool vSync;
        RenderTexture2DData* backbuffer = nullptr;
        DepthTexture2DData* depthBuffer = nullptr;

    private:
#ifdef ST_RENDERER_DX11
        IDXGISwapChain* m_Swapchain;
        UInt8 m_BufferCount;
#endif  // ST_RENDERER_DX11

        friend class SwapchainAPI;
    };
}  // namespace Sentinel