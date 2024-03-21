#ifndef _EXPORT_GRAPHICS_BUFFER_CONSTANTBUFFERDATA_EXPORT_H
#define _EXPORT_GRAPHICS_BUFFER_CONSTANTBUFFERDATA_EXPORT_H

#ifdef _MSC_VER
    #pragma once
#endif  // _MSC_VER

typedef enum CBufferUsageType { DEFAULT = 0, DYNAMIC = 1 } CBufferUsageType;

typedef struct ConstantbufferData {
    void* data;
    unsigned long long size;
    CBufferUsageType usageType;
    unsigned int bindSlot;
    void* native;
} ConstantbufferData;

#endif  // !_EXPORT_GRAPHICS_BUFFER_CONSTANTBUFFERDATA_EXPORT_H
