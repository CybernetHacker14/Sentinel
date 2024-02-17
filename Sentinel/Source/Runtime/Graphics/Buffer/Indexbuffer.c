#include "stpch.h"
#include "_EXPORT/Graphics/RendererAPI_EXPORT.h"
#include "_EXPORT/Graphics/RendererData_EXPORT.h"
#include "Allocator/FixedSlabAllocator.h"
#include "Graphics/Buffer/Indexbuffer.h"
#include "Graphics/Geometry/Geometry.h"

#include <stdlib.h>

#ifdef ST_PLATFORM_WINDOWS
    #include "Graphics/D3D11/Buffer/D3D11Indexbuffer.h"
#endif  // ST_PLATFORM_WINDOWS

typedef struct IndexbufferAPI {
    void (*create)(IndexbufferData*);
    void (*destroy)(IndexbufferData*);
    void (*bind)(IndexbufferData*);
    void (*unbind)(IndexbufferData*);
} IndexbufferAPI;

static IndexbufferAPI iBufferAPI;

static FixedSlabAllocator iBufferAllocator;

static IndexbufferData* registeredIBuffers[MAX_REGISTERED_GEOMETRIES];

void Sentinel_Indexbuffer_Init() {
    if (Sentinel_Renderer_GetBackend() == D3D11) {
        iBufferAPI.create = Sentinel_D3D11Indexbuffer_Create;
        iBufferAPI.destroy = Sentinel_D3D11Indexbuffer_Destroy;
        iBufferAPI.bind = Sentinel_D3D11Indexbuffer_Bind;
        iBufferAPI.unbind = Sentinel_D3D11Indexbuffer_Unbind;
    }

    Sentinel_FixedSlabAllocator_Allocate(&iBufferAllocator, sizeof(IndexbufferData), MAX_REGISTERED_GEOMETRIES);
}

void Sentinel_Indexbuffer_Deinit() {
    IndexbufferData* start = (IndexbufferData*)(iBufferAllocator.startingAddress);
    for (UShort i = 0; i < iBufferAllocator.max; ++i)
        if (start[i].native != NULL) iBufferAPI.destroy(&(start[i]));

    Sentinel_FixedSlabAllocator_DeleteAll(&iBufferAllocator);
    Sentinel_FixedSlabAllocator_Deallocate(&iBufferAllocator);
}

void Sentinel_Indexbuffer_CreateIndexbuffer(Geometry* geometry) {
    UShort index;
    IndexbufferData* iBuffer = (IndexbufferData*)Sentinel_FixedSlabAllocator_New(&iBufferAllocator, &index);
    iBuffer->geometry = geometry;

    registeredIBuffers[index] = iBuffer;

    iBufferAPI.create(iBuffer);
}

void Sentinel_Indexbuffer_BindAllIndexbuffers() {
    IndexbufferData* start = (IndexbufferData*)(iBufferAllocator.startingAddress);
    for (UShort i = 0; i < iBufferAllocator.max; ++i)
        if (start[i].native != NULL) iBufferAPI.bind(&(start[i]));
}

IndexbufferData** Sentinel_Indexbuffer_GetRegisteredIndexbuffers() {
    return registeredIBuffers;
}

UInt Sentinel_Indexbuffer_GetRegisteredIndexbufferCount() {
    UInt count = 0;

    IndexbufferData* start = (IndexbufferData*)(iBufferAllocator.startingAddress);
    for (UShort i = 0; i < iBufferAllocator.max; ++i)
        if (start[i].native != NULL) count += start[i].geometry->indexCount;

    return count;
}
