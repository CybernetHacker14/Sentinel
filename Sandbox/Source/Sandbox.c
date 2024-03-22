#include <EntryPoint.h>

#include <Application/ApplicationAPI_EXPORT.h>
#include <Window/WindowAPI_EXPORT.h>
#include <Window/WindowData_EXPORT.h>

#include <Graphics/Geometry/GeometryAPI_EXPORT.h>
#include <Graphics/RendererAPI_EXPORT.h>
#include <Graphics/RendererData_EXPORT.h>

#include <Graphics/Material/ShaderAPI_EXPORT.h>
#include <Graphics/Material/ShaderDATA_EXPORT.h>

#include <Graphics/Camera/CameraAPI_EXPORT.h>
#include <Graphics/Camera/CameraData_EXPORT.h>

#include <Graphics/Buffer/ConstantbufferAPI_EXPORT.h>
#include <Graphics/Buffer/ConstantbufferData_EXPORT.h>

#include <Graphics/Texture/Texture2DAPI_EXPORT.h>
#include <Graphics/Texture/Texture2DData_EXPORT.h>

#include <Event/EventAPI_EXPORT.h>
#include <Event/EventData_EXPORT.h>

#include <cglm/cglm.h>
#include <stdlib.h>

struct Vertex {
    float x, y, z;
    float u, v;
};

static struct Vertex vertices[] = {
    {-0.5f, -0.5f, 0.7f, 0.0f, 1.0f},
    {-0.5f, 0.5f, 0.7f, 0.0f, 0.0f},
    {0.5f, 0.5f, 0.7f, 1.0f, 0.0f},
    {0.5f, -0.5f, 0.7f, 1.0f, 1.0f}};

static unsigned int indices[] = {0, 1, 2, 2, 3, 0};

static CameraData* camera;

static ConstantbufferData* objectColor;

static Texture2DData* texture;

static short onResizeIndex;

static int OnResize(EventType type, EventData data, void* listener) {
    camera->aspectRatio = (float)(data.UInt16[0]) / (float)(data.UInt16[1]);
    return 0;
}

void Sentinel_Main_Stage_OnStartup() {
    Sentinel_CreateApplicationConfig("Test", "0.0.01");
    Sentinel_WindowAPI_CreateInternalWindow(Sentinel_GetApplicationConfigName(), 800, 600, WINDOWED, 1);

    onResizeIndex = Sentinel_EventAPI_RegisterEvent(WindowResize, NULL, OnResize);
}

void Sentinel_Main_Stage_OnMainLoop_PreBegin() {
    Sentinel_Renderer_Init(D3D11);

    Sentinel_ShaderAPI_CreateFromFile("Source/BaseVertexShader.hlsl", VERTEX);
    Sentinel_ShaderAPI_CreateFromFile("Source/BasePixelShader.hlsl", PIXEL);

    Sentinel_GeometryAPI_SubmitIndividualGeometryData(
        sizeof(struct Vertex), _countof(vertices), vertices, sizeof(unsigned int), _countof(indices), indices);

    vec3 position = {0.0f, 0.0f, 0.0f};
    vec3 rotation = {0.0f, 0.0f, 1.0f};
    camera = Sentinel_CameraAPI_CreateCamera(position, rotation);
    /*camera->projectionMode = ORTHOGRAPHIC;
    camera->aspectRatio = 800.0f / 600.0f;
    camera->orthographicSize = 10.0f;
    camera->near = -100.0f;
    camera->far = 100.0f;*/
    camera->projectionMode = PERSPECTIVE;
    camera->aspectRatio = 800.0f / 600.0f;
    camera->perspectiveFov = glm_rad(90.0f);
    camera->near = 0.001f;
    camera->far = 1000.0f;

    objectColor = Sentinel_ConstantbufferAPI_Create(DEFAULT, 1, sizeof(vec4));
    Sentinel_ConstantbufferAPI_Bind(objectColor, VERTEX);
    // vec4 color = {1.0f, 0.5f, 0.31f, 1.0f};
    vec4 color = {1.0f, 1.0f, 1.0f, 1.0f};
    Sentinel_ConstantbufferAPI_SetData(objectColor, color);

    Texture2DImportSettings settings = {0};
    settings.ReadWriteEnabled = 0;
    settings.sRGB = 0;
    settings.wrapMode = CLAMP;
    texture = Sentinel_Texture2DAPI_CreateFromFile("Assets/wall.jpg", &settings);
    Sentinel_Texture2DAPI_Bind(texture, PIXEL, 0);
}

void Sentinel_Main_Stage_OnMainLoop_Begin() {
}

void Sentinel_Main_Stage_OnMainLoop_OnUpdate() {
    Sentinel_CameraAPI_OnUpdate(camera);
}

void Sentinel_Main_Stage_OnMainLoop_OnRender() {
}

void Sentinel_Main_Stage_OnMainLoop_End() {
}

void Sentinel_Main_Stage_OnMainLoop_PostEnd() {
}

void Sentinel_Main_Stage_OnMainLoop_Shutdown() {
}
