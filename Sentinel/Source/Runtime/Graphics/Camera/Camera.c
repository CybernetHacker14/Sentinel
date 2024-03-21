#include "stpch.h"
#include "_EXPORT/Graphics/Buffer/ConstantbufferAPI_EXPORT.h"
#include "_EXPORT/Graphics/Buffer/ConstantbufferData_EXPORT.h"
#include "_EXPORT/Graphics/Camera/CameraAPI_EXPORT.h"
#include "_EXPORT/Graphics/Camera/CameraData_EXPORT.h"
#include "_EXPORT/Graphics/Material/ShaderDATA_EXPORT.h"
#include "Allocator/FixedSlabAllocator.h"
#include "Graphics/Buffer/Constantbuffer.h"
#include "Graphics/Camera/Camera.h"
#include "Math/MathExtensions_SIMD.h"

#include <cglm/cglm.h>
#include <cglm/clipspace/ortho_rh_zo.h>
#include <cglm/clipspace/persp_rh_zo.h>

#define MAX_CAMERAS 8

// Utilities -- Start

// Still temporary, especially the function names
static const vec3 worldUp = {0.0f, 1.0f, 0.0f};

static void PerspectiveViewMatrix(CameraData* camera, mat4 viewMatrix) {
    vec3 front = {
        Sentinel_MathExtensions_SIMD_Cos(glm_rad(-90 + camera->eulerOrientation[0])) *
            Sentinel_MathExtensions_SIMD_Sin(glm_rad(camera->eulerOrientation[1])),
        Sentinel_MathExtensions_SIMD_Sin(glm_rad(camera->eulerOrientation[1])),
        Sentinel_MathExtensions_SIMD_Sin(glm_rad(-90 + camera->eulerOrientation[0])) *
            Sentinel_MathExtensions_SIMD_Cos(glm_rad(camera->eulerOrientation[2]))};
    glm_normalize(front);

    vec3 right = {0};
    Sentinel_MathExtensions_SIMD_CrossProduct(front, worldUp, right);
    glm_normalize(right);

    vec3 up = {0};
    Sentinel_MathExtensions_SIMD_CrossProduct(right, front, up);
    glm_normalize(up);

    mat4 transform = {0};
    vec3 center = {0};
    glm_vec3_add(camera->position, front, center);
    glm_lookat_rh(camera->position, center, up, transform);
    vec3 x = {1.0f, 0.0f, 0.0f};
    vec3 y = {0.0f, 1.0f, 0.0f};
    vec3 z = {0.0f, 0.0f, 1.0f};
    glm_rotate(transform, camera->eulerOrientation[0], x);
    glm_rotate(transform, camera->eulerOrientation[1], y);
    glm_rotate(transform, camera->eulerOrientation[2], z);

    glm_mat4_inv(transform, viewMatrix);
}

static void OrthographicViewMatrix(CameraData* camera, mat4 viewMatrix) {
    mat4 transform = GLM_MAT4_IDENTITY_INIT;
    glm_translate(transform, camera->position);
    vec3 x = {1.0f, 0.0f, 0.0f};
    vec3 y = {0.0f, 1.0f, 0.0f};
    vec3 z = {0.0f, 0.0f, 1.0f};
    glm_rotate(transform, camera->eulerOrientation[0], x);
    glm_rotate(transform, camera->eulerOrientation[1], y);
    glm_rotate(transform, camera->eulerOrientation[2], z);

    glm_mat4_inv(transform, viewMatrix);
}

static void PerspectiveProjectionMatrix(CameraData* camera, mat4 projMatrix) {
    glm_perspective_rh_zo(camera->perspectiveFov, camera->aspectRatio, camera->near, camera->far, projMatrix);
}

static void OrthographicProjectionMatrix(CameraData* camera, mat4 projMatrix) {
    Float left = -(camera->orthographicSize) * camera->aspectRatio * 0.5f;
    Float right = camera->orthographicSize * camera->aspectRatio * 0.5f;
    Float bottom = -(camera->orthographicSize) * 0.5f;
    Float top = camera->orthographicSize * 0.5f;
    glm_ortho_rh_zo(left, right, bottom, top, camera->near, camera->far, projMatrix);
}

static void (*viewComputePFn[])(CameraData*, mat4) = {PerspectiveViewMatrix, OrthographicViewMatrix};
static void (*projComputePFn[])(CameraData*, mat4) = {PerspectiveProjectionMatrix, OrthographicProjectionMatrix};

static void ViewProjectionMatrix(CameraData* camera) {
    mat4 viewMatrix = GLM_MAT4_IDENTITY_INIT;
    viewComputePFn[(Int)(camera->projectionMode)](camera, viewMatrix);

    mat4 projMatrix = GLM_MAT4_IDENTITY_INIT;
    projComputePFn[(Int)(camera->projectionMode)](camera, projMatrix);

    glm_mat4_mul(viewMatrix, projMatrix, camera->viewProjection);
}

// \Utilities -- End

static FixedSlabAllocator cameraAllocator;

void Sentinel_Camera_Init() {
    Sentinel_FixedSlabAllocator_Allocate(&cameraAllocator, sizeof(CameraData), MAX_CAMERAS);
}

void Sentinel_Camera_Deinit() {
    CameraData* start = (CameraData*)(cameraAllocator.startingAddress);

    for (UShort i = 0; i < MAX_CAMERAS; ++i)
        if (start[i].cameraCBuffer != NULL) Sentinel_ConstantbufferAPI_Destroy(start[i].cameraCBuffer);

    Sentinel_FixedSlabAllocator_DeleteAll(&cameraAllocator);
    Sentinel_FixedSlabAllocator_Deallocate(&cameraAllocator);
}

ST_API CameraData* Sentinel_CameraAPI_CreateCamera(vec3 position, vec3 eulerRotation) {
    UShort index;
    CameraData* camera = (CameraData*)Sentinel_FixedSlabAllocator_New(&cameraAllocator, &index);
    glm_vec3_copy(camera->position, position);
    glm_vec3_copy(camera->eulerOrientation, eulerRotation);
    camera->cameraCBuffer = Sentinel_ConstantbufferAPI_Create(DYNAMIC, index, 16 * sizeof(Float));
    return camera;
}

ST_API void Sentinel_CameraAPI_DeleteCamera(CameraData* camera) {
    UShort index;
    Sentinel_FixedSlabAllocator_Delete(&cameraAllocator, camera, &index);
    Sentinel_ConstantbufferAPI_Destroy(camera->cameraCBuffer);
}

ST_API void Sentinel_CameraAPI_OnUpdate(CameraData* camera) {
    ViewProjectionMatrix(camera);
    Sentinel_ConstantbufferAPI_Bind(camera->cameraCBuffer, VERTEX);
    Sentinel_ConstantbufferAPI_SetData(camera->cameraCBuffer, camera->viewProjection);
}
