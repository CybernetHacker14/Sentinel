#ifndef GRAPHICS_BUFFER_INDEXBUFFER_H
#define GRAPHICS_BUFFER_INDEXBUFFER_H

#ifdef _MSC_VER
    #pragma once
#endif  // _MSC_VER

#include "Common/Core/DataTypes.h"

typedef struct Geometry Geometry;

typedef struct IndexbufferData {
    Geometry* geometry;
    void* native;
} IndexbufferData;

void Sentinel_Indexbuffer_Init();

void Sentinel_Indexbuffer_Deinit();

void Sentinel_Indexbuffer_CreateIndexbuffer(Geometry* geometry);

void Sentinel_Indexbuffer_BindAllIndexbuffers();

IndexbufferData** Sentinel_Indexbuffer_GetRegisteredIndexbuffers();

UInt Sentinel_Indexbuffer_GetRegisteredIndexbufferCount();

#endif  // !GRAPHICS_BUFFER_INDEXBUFFER_H
