#include "stpch.h"

#include "Sentinel/Graphics/Common/Backend.h"
#include "Sentinel/Graphics/Device/SwapchainAPI.h"

#include "Sentinel/Graphics/Common/GraphicsMemoryManager.h"

#include "Platform/DirectX11/Graphics/Device/DX11SwapchainData.h"
#include "Platform/DirectX11/Graphics/Device/DX11SwapchainAPI.h"

namespace Sentinel {
    SwapchainData* SwapchainAPI::CreateSwapchain(
        SharedRef<GraphicsMemoryManager> memoryHandle, ContextData* context, GLFWwindow* windowHandle) {
        switch (Backend::GetAPI()) {
            case Backend::API::DirectX11: {
                SwapchainData* swapchain = memoryHandle->SwapchainAllocator.New<DX11SwapchainData>();
                swapchain->Context = context;
                DX11SwapchainAPI::Init(SwapchainAPI::Cast<DX11SwapchainData>(swapchain), windowHandle);
                return swapchain;
            }
            case Backend::API::None: ST_ENGINE_ASSERT(false, "API::None currently not supported"); return nullptr;
        }
        ST_ENGINE_ASSERT(false, "Unknown Backend API");
        return nullptr;
    }
}  // namespace Sentinel