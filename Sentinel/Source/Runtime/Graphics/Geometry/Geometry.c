#include "stpch.h"
#include "_EXPORT/Graphics/Geometry/GeometryAPI_EXPORT.h"
#include "Allocator/FixedSlabAllocator.h"
#include "Common/Core/DataTypes.h"
#include "Graphics/Buffer/Indexbuffer.h"
#include "Graphics/Buffer/Vertexbuffer.h"
#include "Graphics/Geometry/Geometry.h"

#include "Logging/Log.h"

#include <stdlib.h>

static FixedSlabAllocator allocator;

static Geometry* registeredGeometries[MAX_REGISTERED_GEOMETRIES];

ST_API void Sentinel_GeometryAPI_SubmitIndividualGeometryData(
    unsigned int vertexSize,
    unsigned int vertexCount,
    void* vertices,
    unsigned int indexSize,
    unsigned int indexCount,
    void* indices) {
    UShort index;
    Geometry* geometry = (Geometry*)Sentinel_FixedSlabAllocator_New(&allocator, &index);
    geometry->vertexSize = vertexSize;
    geometry->vertexCount = vertexCount;
    geometry->vertices = Sentinel_Malloc((ULLong)(vertexSize * vertexCount));
    Sentinel_Memcpy(geometry->vertices, vertices, (ULLong)(vertexSize * vertexCount));

    geometry->indexSize = 0;
    geometry->indexCount = 0;
    geometry->indices = NULL;

    if (indices != NULL) {
        geometry->indexSize = indexSize;
        geometry->indexCount = indexCount;
        geometry->indices = Sentinel_Malloc((ULLong)(indexSize * indexCount));
        Sentinel_Memcpy(geometry->indices, indices, (ULLong)(indexSize * indexCount));
    }

    registeredGeometries[index] = geometry;

    Sentinel_Vertexbuffer_CreateVertexbuffer(registeredGeometries[index]);

    if (registeredGeometries[index]->indices) Sentinel_Indexbuffer_CreateIndexbuffer(registeredGeometries[index]);
}

void Sentinel_Geometry_Init() {
    Sentinel_FixedSlabAllocator_Allocate(&allocator, sizeof(Geometry), MAX_REGISTERED_GEOMETRIES);

    Sentinel_Memset(registeredGeometries, 0, (ULLong)(sizeof(Geometry*) * MAX_REGISTERED_GEOMETRIES));
}

void Sentinel_Geometry_Deinit() {
    for (UInt i = 0; i < MAX_REGISTERED_GEOMETRIES; i++) {
        if (registeredGeometries[i]) {
            Sentinel_Free(registeredGeometries[i]->vertices);
            if (registeredGeometries[i]->indices) Sentinel_Free(registeredGeometries[i]->indices);
        }
    }

    Sentinel_FixedSlabAllocator_Deallocate(&allocator);
}

Geometry** Sentinel_GetRegisteredGeometries() {
    return registeredGeometries;
}
