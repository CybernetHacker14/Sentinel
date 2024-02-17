#include "stpch.h"
#include "_EXPORT/Graphics/RendererAPI_EXPORT.h"
#include "_EXPORT/Graphics/RendererData_EXPORT.h"
#include "Allocator/FixedSlabAllocator.h"
#include "Graphics/Buffer/Vertexbuffer.h"
#include "Graphics/Geometry/Geometry.h"

#include <stdlib.h>

#ifdef ST_PLATFORM_WINDOWS
    #include "Graphics/D3D11/Buffer/D3D11Vertexbuffer.h"
#endif  // ST_PLATFORM_WINDOWS

typedef struct VertexbufferAPI {
    void (*create)(VertexbufferData*);
    void (*destroy)(VertexbufferData*);
    void (*bind)(VertexbufferData*);
    void (*unbind)(VertexbufferData*);
} VertexbufferAPI;

static VertexbufferAPI vBufferAPI;

static FixedSlabAllocator vBufferAllocator;

static VertexbufferData* registeredVBuffers[MAX_REGISTERED_GEOMETRIES];

void Sentinel_Vertexbuffer_Init() {
    if (Sentinel_Renderer_GetBackend() == D3D11) {
        vBufferAPI.create = Sentinel_D3D11Vertexbuffer_Create;
        vBufferAPI.destroy = Sentinel_D3D11Vertexbuffer_Destroy;
        vBufferAPI.bind = Sentinel_D3D11Vertexbuffer_Bind;
        vBufferAPI.unbind = Sentinel_D3D11Vertexbuffer_Unbind;
    }

    Sentinel_FixedSlabAllocator_Allocate(&vBufferAllocator, sizeof(VertexbufferData), MAX_REGISTERED_GEOMETRIES);
}

void Sentinel_Vertexbuffer_Deinit() {
    VertexbufferData* start = (VertexbufferData*)(vBufferAllocator.startingAddress);
    for (UShort i = 0; i < vBufferAllocator.max; ++i)
        if (start[i].native != NULL) vBufferAPI.destroy(&(start[i]));

    Sentinel_FixedSlabAllocator_DeleteAll(&vBufferAllocator);
    Sentinel_FixedSlabAllocator_Deallocate(&vBufferAllocator);
}

void Sentinel_Vertexbuffer_CreateVertexbuffer(Geometry* geometry) {
    UShort index;
    VertexbufferData* vBuffer = (VertexbufferData*)Sentinel_FixedSlabAllocator_New(&vBufferAllocator, &index);
    vBuffer->geometry = geometry;

    registeredVBuffers[index] = vBuffer;

    vBufferAPI.create(vBuffer);
}

void Sentinel_Vertexbuffer_BindAllVertexbuffers() {
    VertexbufferData* start = (VertexbufferData*)(vBufferAllocator.startingAddress);
    for (UShort i = 0; i < vBufferAllocator.max; ++i)
        if (start[i].native != NULL) vBufferAPI.bind(&(start[i]));
}

VertexbufferData** Sentinel_Vertexbuffer_GetRegisteredVertexbuffers() {
    return registeredVBuffers;
}

UInt Sentinel_Vertexbuffer_GetRegisteredVertexbufferCount() {
    UInt count = 0;

    VertexbufferData* start = (VertexbufferData*)(vBufferAllocator.startingAddress);
    for (UShort i = 0; i < vBufferAllocator.max; ++i)
        if (start[i].native != NULL) count += start[i].geometry->vertexCount;

    return count;
}
