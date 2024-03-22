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

#include "Window/Window.h"

#include <cglm/cglm.h>
#include <cglm/mat4.h>
#include <cglm/clipspace/view_rh_zo.h>
#include <cglm/clipspace/view_lh_zo.h>
#include <cglm/clipspace/ortho_rh_zo.h>
#include <cglm/clipspace/ortho_lh_zo.h>
#include <cglm/clipspace/persp_rh_zo.h>
#include <cglm/clipspace/persp_lh_zo.h>

#include <GLFW/glfw3.h>

#define MAX_CAMERAS 8

// Utilities -- Start

// A left - handed viewing system is used by default by DirectX: the viewer’s X axis goes to the right,
// the Y axis is up, and Z goes into the screen.This also feels natural,as you label the screen’s X and Y axes
// from the lower left corner of the display and Z increases with depth into the screen.OpenGL’s viewing system
// is right - handed by default, the difference being that + Z goes towards the viewer.

// Still temporary, especially the function names
static const vec3 worldUp = {0.0f, 1.0f, 0.0f};

static void PerspectiveViewMatrix(CameraData* camera, mat4 viewMatrix) {
    vec3 pos = {0.0f, 0.0f, -0.1f};
    vec3 target = {0.0f, 0.0f, 0.0f};
    glm_lookat_lh(pos, target, worldUp, viewMatrix);
}

static void OrthographicViewMatrix(CameraData* camera, mat4 viewMatrix) {
    /*mat4 transform = GLM_MAT4_IDENTITY_INIT;
    glm_translate(transform, camera->position);
    vec3 x = {1.0f, 0.0f, 0.0f};
    vec3 y = {0.0f, 1.0f, 0.0f};
    vec3 z = {0.0f, 0.0f, 1.0f};
    glm_rotate(transform, camera->eulerOrientation[0], x);
    glm_rotate(transform, camera->eulerOrientation[1], y);
    glm_rotate(transform, camera->eulerOrientation[2], z);

    glm_mat4_inv(transform, viewMatrix);*/
}

static void PerspectiveProjectionMatrix(CameraData* camera, mat4 projMatrix) {
    // glm_perspective_rh_zo(camera->perspectiveFov, camera->aspectRatio, camera->near, camera->far, projMatrix);
    glm_perspective_lh_zo(camera->perspectiveFov, camera->aspectRatio, camera->near, camera->far, projMatrix);
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

    glm_mat4_mul(projMatrix, viewMatrix, camera->viewProjection);
    glm_mat4_transpose(camera->viewProjection);
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
    glm_vec3_copy(position, camera->position);
    glm_vec3_copy(eulerRotation, camera->eulerOrientation);
    camera->cameraCBuffer = Sentinel_ConstantbufferAPI_Create(DYNAMIC, index, 16 * sizeof(Float));
    return camera;
}

ST_API void Sentinel_CameraAPI_DeleteCamera(CameraData* camera) {
    UShort index;
    Sentinel_FixedSlabAllocator_Delete(&cameraAllocator, camera, &index);
    Sentinel_ConstantbufferAPI_Destroy(camera->cameraCBuffer);
}

ST_API void Sentinel_CameraAPI_OnUpdate(CameraData* camera) {
    const float cameraSpeed = 0.05f;
    if (glfwGetKey((GLFWwindow*)Sentinel_Window_GetNativeHandle(), GLFW_KEY_W) == GLFW_PRESS)
        camera->position[2] += cameraSpeed;
    if (glfwGetKey((GLFWwindow*)Sentinel_Window_GetNativeHandle(), GLFW_KEY_S) == GLFW_PRESS)
        camera->position[2] -= cameraSpeed;
    if (glfwGetKey((GLFWwindow*)Sentinel_Window_GetNativeHandle(), GLFW_KEY_A) == GLFW_PRESS)
        camera->position[0] -= cameraSpeed;
    if (glfwGetKey((GLFWwindow*)Sentinel_Window_GetNativeHandle(), GLFW_KEY_D) == GLFW_PRESS)
        camera->position[0] += cameraSpeed;

    if (glfwGetKey((GLFWwindow*)Sentinel_Window_GetNativeHandle(), GLFW_KEY_I) == GLFW_PRESS)
        camera->eulerOrientation[1] += cameraSpeed;
    if (glfwGetKey((GLFWwindow*)Sentinel_Window_GetNativeHandle(), GLFW_KEY_K) == GLFW_PRESS)
        camera->eulerOrientation[1] -= cameraSpeed;
    if (glfwGetKey((GLFWwindow*)Sentinel_Window_GetNativeHandle(), GLFW_KEY_J) == GLFW_PRESS)
        camera->eulerOrientation[0] -= cameraSpeed;
    if (glfwGetKey((GLFWwindow*)Sentinel_Window_GetNativeHandle(), GLFW_KEY_L) == GLFW_PRESS)
        camera->eulerOrientation[0] += cameraSpeed;

    ViewProjectionMatrix(camera);
    Sentinel_ConstantbufferAPI_Bind(camera->cameraCBuffer, VERTEX);
    Sentinel_ConstantbufferAPI_SetData(camera->cameraCBuffer, camera->viewProjection);
}
