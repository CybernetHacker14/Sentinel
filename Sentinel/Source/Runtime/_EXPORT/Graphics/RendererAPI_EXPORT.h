#ifndef _EXPORT_GRAPHICS_RENDERERAPI_EXPORT_H
#define _EXPORT_GRAPHICS_RENDERERAPI_EXPORT_H

#ifdef _MSC_VER
    #pragma once
#endif  // _MSC_VER

#include "../ExportConditional.h"

typedef enum RenderingBackend RenderingBackend;

ST_API void Sentinel_Renderer_Init(RenderingBackend backend);

ST_API RenderingBackend Sentinel_Renderer_GetBackend();

#endif  // !_EXPORT_GRAPHICS_RENDERERAPI_EXPORT_H
