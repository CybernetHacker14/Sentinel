#ifndef GRAPHICS_BUFFER_CONSTANTBUFFER_H
#define GRAPHICS_BUFFER_CONSTANTBUFFER_H

#ifdef _MSC_VER
    #pragma once
#endif  // _MSC_VER

#define MAX_REGISTERED_CONSTANTBUFFERS 8

#include "Common/Core/DataTypes.h"

typedef enum CBufferUsageType { DEFAULT = 0, DYNAMIC = 1 } CBufferUsageType;

typedef enum ShaderType ShaderType;

typedef struct ConstantbufferData {
    void* data;
    ULLong size;
    CBufferUsageType usageType;
    UInt bindSlot;
    void* native;
} ConstantbufferData;

void Sentinel_Constantbuffer_Init();

void Sentinel_Constantbuffer_Deinit();

ConstantbufferData* Sentinel_Constantbuffer_Create(CBufferUsageType usage, UInt slot, ULLong size);

void Sentinel_Constantbuffer_Destroy(ConstantbufferData* cBuffer);

void Sentinel_Constantbuffer_Bind(ConstantbufferData* cBuffer, ShaderType shaderStage);

void Sentinel_Constantbuffer_SetData(ConstantbufferData* cBuffer, void* data);

#endif  // !GRAPHICS_BUFFER_CONSTANTBUFFER_H
