#ifndef GRAPHICS_TEXTURE_RENDERTEXTURE2D_H
#define GRAPHICS_TEXTURE_RENDERTEXTURE2D_H

#ifdef _MSC_VER
    #pragma once
#endif  // _MSC_VER

#include "Common/Core/DataTypes.h"

typedef enum ShaderType ShaderType;
typedef enum ColorFormat ColorFormat;

typedef struct RenderTexture2DData RenderTexture2DData;

void Sentinel_RenderTexture2D_Init();

void Sentinel_RenderTexture2D_Deinit();

RenderTexture2DData* Sentinel_RenderTexture2D_CreateFromSwapchain();

#endif  // !GRAPHICS_TEXTURE_RENDERTEXTURE2D_H
