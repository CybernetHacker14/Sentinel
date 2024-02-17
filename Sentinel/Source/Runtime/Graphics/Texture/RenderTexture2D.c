#include "stpch.h"
#include "_EXPORT/Graphics/Material/ShaderData_EXPORT.h"
#include "_EXPORT/Graphics/RendererAPI_EXPORT.h"
#include "_EXPORT/Graphics/RendererData_EXPORT.h"
#include "_EXPORT/Graphics/Texture/RenderTexture2DAPI_EXPORT.h"
#include "_EXPORT/Graphics/Texture/RenderTexture2DData_EXPORT.h"
#include "Allocator/FixedSlabAllocator.h"
#include "Graphics/Device/Swapchain.h"
#include "Graphics/Texture/RenderTexture2D.h"

#include "Window/Window.h"

#include "Logging/Log.h"

#ifdef ST_PLATFORM_WINDOWS
    #include "Graphics/D3D11/Texture/D3D11RenderTexture2D.h"
#endif  // ST_PLATFORM_WINDOWS

#include <stdlib.h>

typedef struct RT2DAPI {
    void (*init)();
    void (*deinit)();
    void (*create)(RenderTexture2DData*);
    void (*swapchaincreate)(RenderTexture2DData*, SwapchainData*);
    void (*destroy)(RenderTexture2DData*);
    void (*renderbind)(RenderTexture2DData*);
    void (*renderunbind)(RenderTexture2DData*);
    void (*shaderbind)(RenderTexture2DData*, UShort, ShaderType);
    void (*shaderunbind)(RenderTexture2DData*);
    void (*clear)(RenderTexture2DData*, Float[4]);
} RT2DAPI;

static RT2DAPI rT2DAPI;

static FixedSlabAllocator rT2DAllocator;

void Sentinel_RenderTexture2D_Init() {
    if (Sentinel_Renderer_GetBackend() == D3D11) {
        rT2DAPI.init = Sentinel_D3D11RenderTexture2D_Init;
        rT2DAPI.deinit = Sentinel_D3D11RenderTexture2D_Deinit;
        rT2DAPI.create = Sentinel_D3D11RenderTexture2D_Create;
        rT2DAPI.swapchaincreate = Sentinel_D3D11RenderTexture2D_CreateFromSwapchain;
        rT2DAPI.destroy = Sentinel_D3D11RenderTexture2D_Destroy;
        rT2DAPI.renderbind = Sentinel_D3D11RenderTexture2D_BindRTV;
        rT2DAPI.renderunbind = Sentinel_D3D11RenderTexture2D_UnbindRTV;
        rT2DAPI.clear = Sentinel_D3D11RenderTexture2D_Clear;
    }

    Sentinel_FixedSlabAllocator_Allocate(&rT2DAllocator, sizeof(RenderTexture2DData), 32);
    rT2DAPI.init();
}

void Sentinel_RenderTexture2D_Deinit() {
    RenderTexture2DData* start = (RenderTexture2DData*)(rT2DAllocator.startingAddress);
    for (UShort i = 0; i < rT2DAllocator.max; ++i)
        if (start[i].native != NULL) rT2DAPI.destroy(&(start[i]));

    rT2DAPI.deinit();

    Sentinel_FixedSlabAllocator_DeleteAll(&rT2DAllocator);
    Sentinel_FixedSlabAllocator_Deallocate(&rT2DAllocator);
}

RenderTexture2DData* Sentinel_RenderTexture2D_CreateFromSwapchain() {
    UShort index = 0;
    RenderTexture2DData* rTData = (RenderTexture2DData*)Sentinel_FixedSlabAllocator_New(&rT2DAllocator, &index);
    rTData->swapchainTarget = 1;
    rTData->width = Sentinel_Window_GetWidth();
    rTData->height = Sentinel_Window_GetHeight();

    rT2DAPI.swapchaincreate(rTData, Sentinel_Swapchain_GetSwapchain());

    return rTData;
}

ST_API RenderTexture2DData* Sentinel_RenderTexture2DAPI_Create(
    unsigned short width, unsigned short height, ColorFormat format) {
    return NULL;
}

ST_API void Sentinel_RenderTexture2DAPI_Destroy(RenderTexture2DData* renderTexture) {
    rT2DAPI.destroy(renderTexture);

    UShort index;
    Sentinel_FixedSlabAllocator_Delete(&rT2DAllocator, renderTexture, &index);
}

ST_API void Sentinel_RenderTexture2DAPI_Clear(RenderTexture2DData* renderTexture, float color[4]) {
    rT2DAPI.clear(renderTexture, color);
}

ST_API void Sentinel_RenderTexture2DAPI_RenderOutputBind(RenderTexture2DData* renderTexture) {
    rT2DAPI.renderbind(renderTexture);
}

ST_API void Sentinel_RenderTexture2DAPI_RenderOutputUnbind(RenderTexture2DData* renderTexture) {
    rT2DAPI.renderunbind(renderTexture);
}
