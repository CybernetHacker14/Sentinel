#ifndef _EXPORT_GRAPHICS_CAMERA_CAMERAAPI_EXPORT_H
#define _EXPORT_GRAPHICS_CAMERA_CAMERAAPI_EXPORT_H

#ifdef _MSC_VER
    #pragma once
#endif  // _MSC_VER

#include "../../ExportConditional.h"

#include <cglm/types.h>

typedef struct CameraData CameraData;

ST_API CameraData* Sentinel_CameraAPI_CreateCamera(vec3 position, vec3 eulerRotation);

ST_API void Sentinel_CameraAPI_DeleteCamera(CameraData* camera);

ST_API void Sentinel_CameraAPI_OnUpdate(CameraData* camera);

#endif  // !_EXPORT_GRAPHICS_CAMERA_CAMERAAPI_EXPORT_H
