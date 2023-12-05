#include <EntryPoint.h>

#include <Window/WindowAPI_EXPORT.h>
#include <Window/WindowData_EXPORT.h>
#include <Application/ApplicationAPI_EXPORT.h>

#include <Graphics/RendererAPI_EXPORT.h>
#include <Graphics/RendererData_EXPORT.h>
#include <Graphics/Geometry/GeometryAPI_EXPORT.h>

#include <Graphics/Material/ShaderDATA_EXPORT.h>
#include <Graphics/Material/ShaderAPI_EXPORT.h>

#include <stdlib.h>

struct Vertex {
    float x, y;
    float r, g, b;
};

static struct Vertex vertices[] = {
    {-0.5f, 0.5f, 1.f, 0.f, 0.f},
    {0.5f, 0.5f, 0.f, 1.f, 0.f},
    {0.5f, -0.5f, 0.f, 1.f, 0.f},
    {-0.5f, -0.5f, 0.f, 0.f, 1.f}};

static unsigned int indices[] = {0, 1, 2, 0, 2, 3};

static const char d3d11_vShader[] =
    "struct VS_INPUT                                \n"
    "{                                              \n"
    "  float2 pos : POSITION;                       \n"
    "  float3 col : COLOR0;                         \n"
    "};                                             \n"
    "                                               \n"
    "struct PS_INPUT                                \n"
    "{                                              \n"
    "  float4 pos : SV_POSITION;                    \n"
    "  float3 col : COLOR0;                         \n"
    "};                                             \n"
    "                                               \n"
    "PS_INPUT vs(VS_INPUT input)                    \n"
    "{                                              \n"
    "  PS_INPUT output;                             \n"
    "  output.pos = float4(input.pos.xy, 0.f, 1.f); \n"
    "  output.col = input.col;                      \n"
    "  return output;                               \n"
    "}                                              \n";

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

void Sentinel_Main_Stage_OnStartup() {
    Sentinel_CreateApplicationConfig("Test", "0.0.01");
    Sentinel_CreateInternalWindow(Sentinel_GetApplicationConfigName(), 800, 600, WINDOWED, 0);
}

void Sentinel_Main_Stage_OnMainLoop_PreBegin() {
    Sentinel_Renderer_Init(D3D11);

    Sentinel_ShaderAPI_CreateFromSource(d3d11_vShader, sizeof(d3d11_vShader), VERTEX);
    Sentinel_ShaderAPI_CreateFromSource(d3d11_pShader, sizeof(d3d11_pShader), PIXEL);

    Sentinel_GeometryAPI_SubmitIndividualGeometryData(
        sizeof(struct Vertex), _countof(vertices), vertices, sizeof(unsigned int), _countof(indices), indices);
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
