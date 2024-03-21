#ifndef _EXPORT_GRAPHICS_CAMERA_CAMERADATA_H
#define _EXPORT_GRAPHICS_CAMERA_CAMERADATA_H

#ifdef _MSC_VER
    #pragma once
#endif  // _MSC_VER

#include <cglm/types.h>

typedef struct ConstantbufferData ConstantbufferData;

typedef enum ProjectionMode { PERSPECTIVE = 0, ORTHOGRAPHIC = 1 } ProjectionMode;

typedef struct CameraData {
    mat4 viewProjection;
    vec3 position;
    ProjectionMode projectionMode;
    vec3 eulerOrientation;
    float aspectRatio;
    float perspectiveFov, orthographicSize;  // Can be combined in a union
    float near, far;
    ConstantbufferData* cameraCBuffer;
} CameraData;

#endif  // !_EXPORT_GRAPHICS_CAMERA_CAMERADATA_H
