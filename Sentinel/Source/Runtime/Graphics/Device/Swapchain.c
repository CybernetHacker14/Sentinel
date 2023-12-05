#include "stpch.h"
#include "_EXPORT/Graphics/Material/ShaderDATA_EXPORT.h"
#include "_EXPORT/Graphics/RendererAPI_EXPORT.h"
#include "_EXPORT/Graphics/RendererData_EXPORT.h"
#include "Graphics/Device/Swapchain.h"

#ifdef ST_PLATFORM_WINDOWS
    #include "Graphics/D3D11/Device/D3D11Swapchain.h"
#endif  // ST_PLATFORM_WINDOWS

typedef struct SwapchainAPI {
    void (*create)(SwapchainData*);
    void (*destroy)(SwapchainData*);
    void (*present)(SwapchainData*);
    void (*resize)(SwapchainData*, UShort, UShort);
} SwapchainAPI;

static SwapchainAPI swapchainAPI;

static SwapchainData swapchain;

void Sentinel_Swapchain_Init() {
    if (Sentinel_Renderer_GetBackend() == D3D11) {
        swapchainAPI.create = Sentinel_D3D11Swapchain_Init;
        swapchainAPI.destroy = Sentinel_D3D11Swapchain_Deinit;
        swapchainAPI.present = Sentinel_D3D11Swapchain_Present;
        swapchainAPI.resize = Sentinel_D3D11Swapchain_Resize;
    }

    swapchainAPI.create(&swapchain);
}

void Sentinel_Swapchain_Deinit() {
    swapchainAPI.destroy(&swapchain);
}

void Sentinel_Swapchain_Present() {
    swapchainAPI.present(&swapchain);
}

void Sentinel_Swapchain_Resize(UShort width, UShort height) {
    swapchainAPI.resize(&swapchain, width, height);
}

SwapchainData* Sentinel_Swapchain_GetSwapchain() {
    return &swapchain;
}
