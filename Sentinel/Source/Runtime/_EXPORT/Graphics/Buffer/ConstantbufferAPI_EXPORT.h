#ifndef _EXPORT_GRAPHICS_BUFFER_CONSTANTBUFFERAPI_EXPORT_H
#define _EXPORT_GRAPHICS_BUFFER_CONSTANTBUFFERAPI_EXPORT_H

#ifdef _MSC_VER
    #pragma once
#endif  // _MSC_VER

#include "../../ExportConditional.h"

typedef enum ShaderType ShaderType;
typedef enum CBUfferUsageType CBufferUsageType;

typedef struct ConstantbufferData ConstantbufferData;

ST_API ConstantbufferData* Sentinel_ConstantbufferAPI_Create(
    CBufferUsageType usage, unsigned int slot, unsigned long long size);

ST_API void Sentinel_ConstantbufferAPI_Destroy(ConstantbufferData* cBuffer);

ST_API void Sentinel_ConstantbufferAPI_Bind(ConstantbufferData* cBuffer, ShaderType shaderStage);

ST_API void Sentinel_ConstantbufferAPI_SetData(ConstantbufferData* cBuffer, void* data);

#endif  // !_EXPORT_GRAPHICS_BUFFER_CONSTANTBUFFERAPI_EXPORT_H
