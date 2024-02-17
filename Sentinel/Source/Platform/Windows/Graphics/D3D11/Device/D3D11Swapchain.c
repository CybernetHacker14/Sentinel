#include "stpch.h"
#include "Graphics/D3D11/Device/D3D11Device.h"
#include "Graphics/D3D11/Device/D3D11Swapchain.h"
#include "Graphics/Device/Swapchain.h"
#include "Window/Window.h"

#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

#define D3D11_NO_HELPERS
#define CINTERFACE
#define COBJMACROS

#include <d3d11.h>
#include <dxgi.h>

#pragma warning(push)
#pragma warning(disable : 6011)

// void Sentinel_D3D11Swapchain_GetBackbufferTexture(Int width, Int height, ID3D11Texture2D** outTexture) {
//     IDXGISwapChain_ResizeBuffers(swapchain, 0, width, height, DXGI_FORMAT_UNKNOWN, 0);
//     IDXGISwapChain_GetBuffer(swapchain, 0, &IID_ID3D11Texture2D, (void**)outTexture);
// }

void Sentinel_D3D11Swapchain_Init(SwapchainData* swapchain) {
    IDXGISwapChain* dxswapchain;

    DXGI_SWAP_CHAIN_DESC desc;
    SecureZeroMemory(&desc, sizeof(DXGI_SWAP_CHAIN_DESC));
    desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
    desc.BufferDesc.RefreshRate.Numerator = 60;
    desc.BufferDesc.RefreshRate.Denominator = 1;

    desc.SampleDesc.Count = 1;
    desc.SampleDesc.Quality = 0;
    desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    desc.OutputWindow = glfwGetWin32Window((GLFWwindow*)(Sentinel_Window_GetNativeHandle()));
    desc.Windowed = TRUE;

    desc.BufferCount = 2;
    desc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
    desc.Flags = 0;

    IDXGIFactory_CreateSwapChain(
        Sentinel_D3D11Device_GetFactory(), (IUnknown*)Sentinel_D3D11Device_GetDevice(), &desc, &dxswapchain);
    IDXGIFactory_MakeWindowAssociation(
        Sentinel_D3D11Device_GetFactory(),
        glfwGetWin32Window((GLFWwindow*)(Sentinel_Window_GetNativeHandle())),
        DXGI_MWA_NO_WINDOW_CHANGES | DXGI_MWA_NO_ALT_ENTER);

    swapchain->native = dxswapchain;
}

void Sentinel_D3D11Swapchain_Deinit(SwapchainData* swapchain) {
    IDXGISwapChain* dxswapchain = (IDXGISwapChain*)(swapchain->native);
    IDXGISwapChain_Release(dxswapchain);
    swapchain->native = NULL;
}

void Sentinel_D3D11Swapchain_Present(SwapchainData* swapchain) {
    IDXGISwapChain* dxswapchain = (IDXGISwapChain*)(swapchain->native);
    IDXGISwapChain_Present(dxswapchain, Sentinel_Window_GetVSync(), 0);
}

void Sentinel_D3D11Swapchain_Resize(SwapchainData* swapchain, UShort width, UShort height) {
    IDXGISwapChain* dxswapchain = (IDXGISwapChain*)(swapchain->native);
    IDXGISwapChain_ResizeBuffers(dxswapchain, 3, width, height, DXGI_FORMAT_UNKNOWN, 0);
}

#pragma warning(pop)
