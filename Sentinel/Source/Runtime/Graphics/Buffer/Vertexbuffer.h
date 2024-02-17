#ifndef GRAPHICS_BUFFER_VERTEXBUFFER_H
#define GRAPHICS_BUFFER_VERTEXBUFFER_H

#ifdef _MSC_VER
    #pragma once
#endif  // _MSC_VER

#include "Common/Core/DataTypes.h"

typedef struct Geometry Geometry;

typedef struct VertexbufferData {
    Geometry* geometry;
    void* native;
} VertexbufferData;

void Sentinel_Vertexbuffer_Init();

void Sentinel_Vertexbuffer_Deinit();

void Sentinel_Vertexbuffer_CreateVertexbuffer(Geometry* geometry);

void Sentinel_Vertexbuffer_BindAllVertexbuffers();

VertexbufferData** Sentinel_Vertexbuffer_GetRegisteredVertexbuffers();

UInt Sentinel_Vertexbuffer_GetRegisteredVertexbufferCount();

#endif  // !GRAPHICS_BUFFER_VERTEXBUFFER_H
