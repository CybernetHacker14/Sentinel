#ifndef WINDOWS_GRAPHICS_D3D11_OUTPUT_D3D11VIEWPORT_H
#define WINDOWS_GRAPHICS_D3D11_OUTPUT_D3D11VIEWPORT_H

#ifdef _MSC_VER
    #pragma once
#endif  // _MSC_VER

#include "Common/Core/DataTypes.h"

typedef struct ViewportData ViewportData;

void Sentinel_D3D11Viewport_Create(ViewportData* viewport);

void Sentinel_D3D11Viewport_Bind(ViewportData* viewport);

void Sentinel_D3D11Viewport_Resize(ViewportData* viewport, UShort width, UShort height);

#endif  // !WINDOWS_GRAPHICS_D3D11_OUTPUT_D3D11VIEWPORT_H
