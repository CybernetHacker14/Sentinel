#ifndef GRAPHICS_OUTPUT_VIEWPORT_H
#define GRAPHICS_OUTPUT_VIEWPORT_H

#ifdef _MSC_VER
    #pragma once
#endif  // _MSC_VER

#include "Common/Core/DataTypes.h"

typedef struct ViewportData {
    Float topX;
    Float topY;
    UShort width;
    UShort height;
    UShort minDepth;
    UShort maxDepth;
} ViewportData;

void Sentinel_Viewport_Init(UShort width, UShort height);

void Sentinel_Viewport_Bind();

void Sentinel_Viewport_Resize(UShort width, UShort height);

#endif  // !GRAPHICS_OUTPUT_VIEWPORT_H
