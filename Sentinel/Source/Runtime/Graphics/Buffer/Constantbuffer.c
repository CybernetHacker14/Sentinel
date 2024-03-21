#include "stpch.h"
#include "_EXPORT/Graphics/Buffer/ConstantbufferAPI_EXPORT.h"
#include "_EXPORT/Graphics/Buffer/ConstantbufferData_EXPORT.h"
#include "_EXPORT/Graphics/Material/ShaderData_EXPORT.h"
#include "_EXPORT/Graphics/RendererAPI_EXPORT.h"
#include "_EXPORT/Graphics/RendererData_EXPORT.h"
#include "Allocator/FixedSlabAllocator.h"
#include "Graphics/Buffer/Constantbuffer.h"

#include <stdlib.h>

#ifdef ST_PLATFORM_WINDOWS
    #include "Graphics/D3D11/Buffer/D3D11Constantbuffer.h"
#endif  // ST_PLATFORM_WINDOWS

#define MAX_REGISTERED_CONSTANTBUFFERS 64

typedef struct ConstantbufferAPI {
    void (*create)(ConstantbufferData*);
    void (*destroy)(ConstantbufferData*);
    void (*bind)(ConstantbufferData*, ShaderType);
    void (*setdata)(ConstantbufferData*, void*);
} ConstantbufferAPI;

static ConstantbufferAPI cBufferAPI;

static FixedSlabAllocator cBufferAllocator;

void Sentinel_Constantbuffer_Init() {
    if (Sentinel_Renderer_GetBackend() == D3D11) {
        cBufferAPI.create = Sentinel_D3D11Constantbuffer_Create;
        cBufferAPI.destroy = Sentinel_D3D11Constantbuffer_Destroy;
        cBufferAPI.bind = Sentinel_D3D11Constantbuffer_Bind;
        cBufferAPI.setdata = Sentinel_D3D11Constantbuffer_SetData;
    }

    Sentinel_FixedSlabAllocator_Allocate(&cBufferAllocator, sizeof(ConstantbufferData), MAX_REGISTERED_CONSTANTBUFFERS);
}

void Sentinel_Constantbuffer_Deinit() {
    ConstantbufferData* start = (ConstantbufferData*)(cBufferAllocator.startingAddress);
    for (UShort i = 0; i < cBufferAllocator.max; ++i)
        if (start[i].native != NULL) cBufferAPI.destroy(&(start[i]));

    Sentinel_FixedSlabAllocator_DeleteAll(&cBufferAllocator);
    Sentinel_FixedSlabAllocator_Deallocate(&cBufferAllocator);
}

ST_API ConstantbufferData* Sentinel_ConstantbufferAPI_Create(
    CBufferUsageType usage, unsigned int slot, unsigned long long size) {
    UShort index;
    ConstantbufferData* buffer = (ConstantbufferData*)Sentinel_FixedSlabAllocator_New(&cBufferAllocator, &index);
    buffer->usageType = usage;
    buffer->bindSlot = slot;
    buffer->size = size;

    cBufferAPI.create(buffer);

    return buffer;
}

ST_API void Sentinel_ConstantbufferAPI_Destroy(ConstantbufferData* cBuffer) {
    cBufferAPI.destroy(cBuffer);
    UShort index;
    Sentinel_FixedSlabAllocator_Delete(&cBufferAllocator, cBuffer, &index);
}

ST_API void Sentinel_ConstantbufferAPI_Bind(ConstantbufferData* cBuffer, ShaderType shaderStage) {
    cBufferAPI.bind(cBuffer, shaderStage);
}

ST_API void Sentinel_ConstantbufferAPI_SetData(ConstantbufferData* cBuffer, void* data) {
    cBufferAPI.setdata(cBuffer, data);
}
