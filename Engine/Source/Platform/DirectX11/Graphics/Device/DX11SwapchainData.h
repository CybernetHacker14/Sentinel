#pragma once

#include "Sentinel/Graphics/Device/SwapchainData.h"

#include <d3d11.h>

struct IDXGISwapChain;

namespace Sentinel {
    class DX11SwapchainData final: public SwapchainData {
    private:
        IDXGISwapChain* m_Swapchain;

        UInt32 m_BufferCount;

        // TEMP
        D3D11_VIEWPORT m_Viewport;

    private:
        friend class DX11SwapchainAPI;
    };
}  // namespace Sentinel