#ifndef WINDOWS_GRAPHICS_D3D11_DEVICE_D3D11SWAPCHAIN_H
#define WINDOWS_GRAPHICS_D3D11_DEVICE_D3D11SWAPCHAIN_H

#ifdef _MSC_VER
    #pragma once
#endif  // _MSC_VER

typedef struct SwapchainData SwapchainData;

void Sentinel_D3D11Swapchain_Init(SwapchainData* swapchain);

void Sentinel_D3D11Swapchain_Deinit(SwapchainData* swapchain);

void Sentinel_D3D11Swapchain_Present(SwapchainData* swapchain);

void Sentinel_D3D11Swapchain_Resize(SwapchainData* swapchain, UShort width, UShort height);

#endif  // !WINDOWS_GRAPHICS_D3D11_DEVICE_D3D11SWAPCHAIN_H
