#include "stpch.h"
#include "_EXPORT/Graphics/RendererAPI_EXPORT.h"
#include "_EXPORT/Graphics/RendererData_EXPORT.h"
#include "_EXPORT/Graphics/Texture/RenderTexture2DAPI_EXPORT.h"
#include "_EXPORT/Graphics/Texture/RenderTexture2DData_EXPORT.h"

#include "Window/Window.h"

#include "Graphics/Device/Swapchain.h"
#include "Graphics/Texture/RenderTexture2D.h"

#include "Graphics/Buffer/Vertexbuffer.h"
#include "Graphics/Buffer/Indexbuffer.h"
#include "Graphics/Geometry/Geometry.h"
#include "Graphics/Material/Shader.h"

#include "Graphics/Output/Viewport.h"

#ifdef ST_PLATFORM_WINDOWS
    #include "Graphics/D3D11/D3D11Renderer.h"
    #include "Renderer.h"
#endif  // ST_PLATFORM_WINDOWS

typedef struct RendererAPI {
    RenderingBackend Backend;

    void (*init)();
    void (*preupdateinit)();
    void (*update)();
    void (*render)();
    void (*deinit)();
} RendererAPI;

static RendererAPI renderAPI;

static RenderTexture2DData* swapchainRT;

ST_API void Sentinel_Renderer_Init(RenderingBackend backend) {
    if (backend == D3D11) {
        renderAPI.init = Sentinel_D3D11Renderer_Init;
        renderAPI.preupdateinit = Sentinel_D3D11Renderer_PreUpdateInit;
        renderAPI.update = Sentinel_D3D11Renderer_OnUpdate;
        renderAPI.render = Sentinel_D3D11Renderer_OnRender;
        renderAPI.deinit = Sentinel_D3D11Renderer_Deinit;
    }

    renderAPI.Backend = backend;

    renderAPI.init();

    Sentinel_Swapchain_Init();
    Sentinel_RenderTexture2D_Init();

    swapchainRT = Sentinel_RenderTexture2D_CreateFromSwapchain();

    Sentinel_Shader_Init();
    Sentinel_Geometry_Init();
    Sentinel_Vertexbuffer_Init();
    Sentinel_Indexbuffer_Init();

    Sentinel_RenderTexture2DAPI_RenderOutputBind(swapchainRT);

    Sentinel_Viewport_Init(Sentinel_Window_GetWidth(), Sentinel_Window_GetHeight());
    Sentinel_Viewport_Bind();
}

ST_API RenderingBackend Sentinel_Renderer_GetBackend() {
    return renderAPI.Backend;
}

void Sentinel_Renderer_MainLoop_PreBegin() {
    renderAPI.preupdateinit();

    Sentinel_Shader_Bind();
    Sentinel_Vertexbuffer_BindAllVertexbuffers();
    Sentinel_Indexbuffer_BindAllIndexbuffers();
}

void Sentinel_Renderer_MainLoop_Update() {
    renderAPI.update();
}

void Sentinel_Renderer_MainLoop_Render() {
    Float clearColor[] = {100.f / 255.f, 149.f / 255.f, 237.f / 255.f, 1.0f};
    Sentinel_RenderTexture2DAPI_Clear(swapchainRT, clearColor);
    renderAPI.render();
    Sentinel_Swapchain_Present();
}

void Sentinel_Renderer_Deinit() {
    Sentinel_RenderTexture2DAPI_RenderOutputUnbind(swapchainRT);

    Sentinel_Indexbuffer_Deinit();
    Sentinel_Vertexbuffer_Deinit();
    Sentinel_Geometry_Deinit();
    Sentinel_Shader_Deinit();

    Sentinel_RenderTexture2D_Deinit();
    Sentinel_Swapchain_Deinit();

    renderAPI.deinit();
}
