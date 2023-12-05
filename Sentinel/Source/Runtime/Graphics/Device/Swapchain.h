#ifndef GRAPHICS_DEVICE_SWAPCHAIN_H
#define GRAPHICS_DEVICE_SWAPCHAIN_H

#ifdef _MSC_VER
    #pragma once
#endif  // _MSC_VER

#include "Common/Core/DataTypes.h"

typedef struct SwapchainData {
    void* native;
} SwapchainData;

void Sentinel_Swapchain_Init();

void Sentinel_Swapchain_Deinit();

void Sentinel_Swapchain_Present();

void Sentinel_Swapchain_Resize(UShort width, UShort height);

SwapchainData* Sentinel_Swapchain_GetSwapchain();

#endif  // !GRAPHICS_DEVICE_SWAPCHAIN_H
