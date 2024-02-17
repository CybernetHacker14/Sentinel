#include "stpch.h"
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

static FixedSlabAllocator cameraAllocator;

static ConstantbufferData* cameraCBuffers[MAX_CAMERAS] = {0};

void Sentinel_Camera_Init() {
    Sentinel_FixedSlabAllocator_Allocate(&cameraAllocator, sizeof(CameraData), MAX_CAMERAS);
}

void Sentinel_Camera_Deinit() {
    CameraData* start = (CameraData*)(cameraAllocator.startingAddress);

    for (UShort i = 0; i < MAX_CAMERAS; ++i)
        if (cameraCBuffers[i] != NULL) Sentinel_Constantbuffer_Destroy(cameraCBuffers[i]);

    Sentinel_FixedSlabAllocator_DeleteAll(&cameraAllocator);
    Sentinel_FixedSlabAllocator_Deallocate(&cameraAllocator);
}

void Sentinel_Camera_OnUpdate() {
    // TEMP CODE, need to remove
    CameraData* start = (CameraData*)(cameraAllocator.startingAddress);

    for (UShort i = 0; i < MAX_CAMERAS; ++i) {
        if (cameraCBuffers[i] != NULL) {
            Sentinel_Camera_ComputeCameraViewProjectionMatrix(&(start[i]));
            Sentinel_Constantbuffer_Bind(cameraCBuffers[i], VERTEX);
            Sentinel_Constantbuffer_SetData(cameraCBuffers[i], start[i].viewProjection);
        }
    }
}

void Sentinel_Camera_ComputeCameraViewProjectionMatrix(CameraData* camera) {
    mat4 viewMatrix = GLM_MAT4_IDENTITY_INIT;
    viewComputePFn[(Int)(camera->projectionMode)](camera, viewMatrix);

    mat4 projMatrix = GLM_MAT4_IDENTITY_INIT;
    projComputePFn[(Int)(camera->projectionMode)](camera, projMatrix);

    glm_mat4_mul(viewMatrix, projMatrix, camera->viewProjection);
}

ST_API CameraData* Sentinel_CameraAPI_CreateCamera(vec3 position, vec3 eulerRotation) {
    UShort index;
    CameraData* camera = (CameraData*)Sentinel_FixedSlabAllocator_New(&cameraAllocator, &index);
    glm_vec3_copy(camera->position, position);
    glm_vec3_copy(camera->eulerOrientation, eulerRotation);
    cameraCBuffers[index] = Sentinel_Constantbuffer_Create(DYNAMIC, index, 16 * sizeof(Float));
    return camera;
}

ST_API void Sentinel_CameraAPI_DeleteCamera(CameraData* camera) {
    UShort index;
    Sentinel_FixedSlabAllocator_Delete(&cameraAllocator, camera, &index);
    Sentinel_Constantbuffer_Destroy(cameraCBuffers[index]);
}
