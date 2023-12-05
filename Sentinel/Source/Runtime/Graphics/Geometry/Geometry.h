#ifndef GRAPHICS_GEOMETRY_GEOMETRY_H
#define GRAPHICS_GEOMETRY_GEOMETRY_H

#ifdef _MSC_VER
    #pragma once
#endif  // _MSC_VER

#define MAX_REGISTERED_GEOMETRIES 64

typedef struct Geometry {
    UInt vertexSize;
    UInt vertexCount;
    void* vertices;
    UInt indexSize;
    UInt indexCount;
    void* indices;
} Geometry;

void Sentinel_Geometry_Init();

void Sentinel_Geometry_Deinit();

Geometry** Sentinel_GetRegisteredGeometries();

#endif  // !GRAPHICS_GEOMETRY_GEOMETRY_H
