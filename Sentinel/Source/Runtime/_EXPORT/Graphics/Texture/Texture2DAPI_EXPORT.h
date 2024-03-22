#ifndef _EXPORT_GRAPHICS_TEXTURE_TEXTURE2DAPI_EXPORT_H
#define _EXPORT_GRAPHICS_TEXTURE_TEXTURE2DAPI_EXPORT_H

#ifdef _MSC_VER
    #pragma once
#endif  // _MSC_VER

#include "../../ExportConditional.h"

typedef struct Texture2DData Texture2DData;
typedef struct Texture2DImportSettings Texture2DImportSettings;

typedef enum TextureWrapMode TextureWrapMode;
typedef enum ShaderType ShaderType;

ST_API Texture2DData* Sentinel_Texture2DAPI_CreateFromFile(const char* filepath, Texture2DImportSettings* settings);

// [[PLACEHOLDER FUNCTION]] Need to flesh it out further
ST_API Texture2DData* Sentinel_Texture2DAPI_CreateFromPixelData();

ST_API void Sentinel_Texture2DAPI_Destroy(Texture2DData* texture);

ST_API void Sentinel_Texture2DAPI_Bind(Texture2DData* texture, ShaderType bindType, unsigned short bindSlot);

ST_API void Sentinel_Texture2DAPI_Unbind(Texture2DData* texture);

ST_API int Sentinel_Texture2DAPI_IsReadWriteEnabled(Texture2DData* texture);

ST_API int Sentinel_Texture2DAPI_IsHDR(Texture2DData* texture);

ST_API int Sentinel_Texture2DAPI_IssRGB(Texture2DData* texture);

#endif  // !_EXPORT_GRAPHICS_TEXTURE_TEXTURE2DAPI_EXPORT_H
