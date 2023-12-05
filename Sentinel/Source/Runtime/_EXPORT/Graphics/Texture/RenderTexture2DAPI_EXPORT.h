#ifndef _EXPORT_GRAPHICS_TEXTURE_RENDERTEXTURE2DAPI_EXPORT_H
#define _EXPORT_GRAPHICS_TEXTURE_RENDERTEXTURE2DAPI_EXPORT_H

#ifdef _MSC_VER
    #pragma once
#endif  // _MSC_VER

#include "../../ExportConditional.h"

typedef struct RenderTexture2DData RenderTexture2DData;
typedef enum ColorFormat ColorFormat;

ST_API RenderTexture2DData* Sentinel_RenderTexture2DAPI_Create(
    unsigned short width, unsigned short height, ColorFormat format);

ST_API void Sentinel_RenderTexture2DAPI_Destroy(RenderTexture2DData* renderTexture);

ST_API void Sentinel_RenderTexture2DAPI_Clear(RenderTexture2DData* renderTexture, float color[4]);

ST_API void Sentinel_RenderTexture2DAPI_RenderOutputBind(RenderTexture2DData* renderTexture);

ST_API void Sentinel_RenderTexture2DAPI_RenderOutputUnbind(RenderTexture2DData* renderTexture);

#endif  // !_EXPORT_GRAPHICS_TEXTURE_RENDERTEXTURE2DAPI_EXPORT_H
