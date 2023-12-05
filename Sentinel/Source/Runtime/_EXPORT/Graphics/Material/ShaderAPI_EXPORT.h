#ifndef _EXPORT_GRAPHICS_MATERIAL_SHADERAPI_EXPORT_H
#define _EXPORT_GRAPHICS_MATERIAL_SHADERAPI_EXPORT_H

#ifdef _MSC_VER
    #pragma once
#endif  // _MSC_VER

#include "../../ExportConditional.h"

typedef enum ShaderType ShaderType;

ST_API void Sentinel_ShaderAPI_CreateFromSource(const char* source, unsigned long long length, ShaderType type);

ST_API void Sentinel_ShaderAPI_CreateFromFile(const char* filepath, ShaderType type);

#endif  // !_EXPORT_GRAPHICS_MATERIAL_SHADERAPI_EXPORT_H
