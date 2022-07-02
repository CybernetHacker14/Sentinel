#pragma once

#include "Sentinel/Graphics/Device/SwapchainData.h"

struct IDXGISwapChain;

namespace Sentinel {
    class DX11SwapchainData final: public SwapchainData {
    private:
        IDXGISwapChain* m_Swapchain;

    private:
        friend class DX11SwapchainAPI;
    };
}  // namespace Sentinel