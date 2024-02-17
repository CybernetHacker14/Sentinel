#ifndef GRAPHICS_CAMERA_CAMERA_H
#define GRAPHICS_CAMERA_CAMERA_H

#ifdef _MSC_VER
    #pragma once
#endif  // _MSC_VER

typedef struct CameraData CameraData;

void Sentinel_Camera_Init();

void Sentinel_Camera_Deinit();

void Sentinel_Camera_OnUpdate();

void Sentinel_Camera_ComputeCameraViewProjectionMatrix(CameraData* camera);

#endif  // !GRAPHICS_CAMERA_CAMERA_H
