#ifndef _EXPORT_GRAPHICS_GEOMETRY_GEOMETRYAPI_H
#define _EXPORT_GRAPHICS_GEOMETRY_GEOMETRYAPI_H

#ifdef _MSC_VER
    #pragma once
#endif  // _MSC_VER

#include "../../ExportConditional.h"

// Investigate - https://github.com/travisvroman/kohi/blob/main/engine/src/renderer/renderer_frontend.c#L312

ST_API void Sentinel_GeometryAPI_SubmitIndividualGeometryData(
    unsigned int vertexSize,
    unsigned int vertexCount,
    void* vertices,
    unsigned int indexSize,
    unsigned int indexCount,
    void* indices);

#endif  // !_EXPORT_GRAPHICS_GEOMETRY_GEOMETRYAPI_H
