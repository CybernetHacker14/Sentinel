#include <EntryPoint.h>

#include <Window/WindowAPI_EXPORT.h>
#include <Window/WindowData_EXPORT.h>
#include <Application/ApplicationAPI_EXPORT.h>

#include <Graphics/RendererAPI_EXPORT.h>
#include <Graphics/RendererData_EXPORT.h>
#include <Graphics/Geometry/GeometryAPI_EXPORT.h>

#include <Graphics/Material/ShaderDATA_EXPORT.h>
#include <Graphics/Material/ShaderAPI_EXPORT.h>

#include <Graphics/Camera/CameraData_EXPORT.h>
#include <Graphics/Camera/CameraAPI_EXPORT.h>

#include <Event/EventData_EXPORT.h>
#include <Event/EventAPI_EXPORT.h>

#include <cglm/cglm.h>
#include <stdlib.h>

struct Vertex {
    float x, y, z;
    float r, g, b;
};

static struct Vertex vertices[] = {
    {-3.0f, 3.0f, -7.0f, 1.f, 0.f, 0.f},
    {-1.0f, 3.0f, -7.0f, 0.f, 1.f, 0.f},
    {-1.0f, 1.0f, -7.0f, 0.f, 1.f, 0.f},
    {-3.0f, 1.0f, -7.0f, 0.f, 0.f, 1.f},
    {1.0f, 1.0f, -5.0f, 1.f, 0.f, 0.f},
    {3.0f, 1.0f, -5.0f, 0.f, 1.f, 0.f},
    {3.0f, -1.0f, -5.0f, 0.f, 1.f, 0.f},
    {1.0f, -1.0f, -5.0f, 0.f, 0.f, 1.f}};

static unsigned int indices[] = {0, 1, 2, 0, 2, 3, 4, 5, 6, 4, 6, 7};

static const char d3d11_vShader[] =
    "#pragma pack_matrix(row_major)                                 \n"
    "cbuffer Camera: register(b0) {                                 \n"
    "  float4x4 u_ViewProjection;                                   \n"
    "}                                                              \n"
    "                                                               \n"
    "struct VS_INPUT                                                \n"
    "{                                                              \n"
    "  float3 pos : POSITION;                                       \n"
    "  float3 col : COLOR0;                                         \n"
    "};                                                             \n"
    "                                                               \n"
    "struct PS_INPUT                                                \n"
    "{                                                              \n"
    "  float4 pos : SV_POSITION;                                    \n"
    "  float3 col : COLOR0;                                         \n"
    "};                                                             \n"
    "                                                               \n"
    "PS_INPUT vs(VS_INPUT input)                                    \n"
    "{                                                              \n"
    "  PS_INPUT output;                                             \n"
    "  output.pos = float4(input.pos.xyz, 1.f);                     \n"
    "  output.col = input.col;                                      \n"
    "  output.pos = mul(output.pos, u_ViewProjection);              \n"
    "  return output;                                               \n"
    "}                                                              \n";

static const char d3d11_pShader[] =
    "struct PS_INPUT                                \n"
    "{                                              \n"
    "  float4 pos : SV_POSITION;                    \n"
    "  float3 col : COLOR0;                         \n"
    "};                                             \n"
    "                                               \n"
    "float4 ps(PS_INPUT input) : SV_Target          \n"
    "{                                              \n"
    "  return float4(input.col, 1.f);               \n"
    "}                                              \n";

static CameraData* camera;

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

    Sentinel_ShaderAPI_CreateFromSource(d3d11_vShader, sizeof(d3d11_vShader), VERTEX);
    Sentinel_ShaderAPI_CreateFromSource(d3d11_pShader, sizeof(d3d11_pShader), PIXEL);

    Sentinel_GeometryAPI_SubmitIndividualGeometryData(
        sizeof(struct Vertex), _countof(vertices), vertices, sizeof(unsigned int), _countof(indices), indices);

    vec3 position = {0.0f, 0.0f, 0.0f};
    vec3 rotation = {0.0f, 0.0f, 0.0f};
    camera = Sentinel_CameraAPI_CreateCamera(position, rotation);
    /*camera->projectionMode = ORTHOGRAPHIC;
    camera->aspectRatio = 800.0f / 600.0f;
    camera->orthographicSize = 10.0f;
    camera->near = -100.0f;
    camera->far = 100.0f;*/
    camera->projectionMode = PERSPECTIVE;
    camera->aspectRatio = 800.0f / 600.0f;
    camera->perspectiveFov = 45.0f;
    camera->near = 0.01f;
    camera->far = 1000.0f;
}

void Sentinel_Main_Stage_OnMainLoop_Begin() {
}

void Sentinel_Main_Stage_OnMainLoop_OnUpdate() {
}

void Sentinel_Main_Stage_OnMainLoop_OnRender() {
}

void Sentinel_Main_Stage_OnMainLoop_End() {
}

void Sentinel_Main_Stage_OnMainLoop_PostEnd() {
}

void Sentinel_Main_Stage_OnMainLoop_Shutdown() {
}
