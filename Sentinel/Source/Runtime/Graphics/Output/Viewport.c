#include "stpch.h"
#include "_EXPORT/Graphics/RendererAPI_EXPORT.h"
#include "_EXPORT/Graphics/RendererData_EXPORT.h"
#include "Graphics/Output/Viewport.h"

#ifdef ST_PLATFORM_WINDOWS
    #include "Graphics/D3D11/Output/D3D11Viewport.h"
#endif  // ST_PLATFORM_WINDOWS

typedef struct ViewportAPI {
    void (*create)(ViewportData*);
    void (*bind)(ViewportData*);
    void (*resize)(ViewportData*, UShort, UShort);
} ViewportAPI;

static ViewportAPI viewportAPI;

static ViewportData viewport;

void Sentinel_Viewport_Init(UShort width, UShort height) {
    if (Sentinel_Renderer_GetBackend() == D3D11) {
        viewportAPI.create = Sentinel_D3D11Viewport_Create;
        viewportAPI.bind = Sentinel_D3D11Viewport_Bind;
        viewportAPI.resize = Sentinel_D3D11Viewport_Resize;
    }

    viewport.topX = 0.0f;
    viewport.topX = 0.0f;
    viewport.width = width;
    viewport.height = height;
    viewport.minDepth = 0;
    viewport.maxDepth = 1;

    viewportAPI.create(&viewport);
}

void Sentinel_Viewport_Bind() {
    viewportAPI.bind(&viewport);
}

void Sentinel_Viewport_Resize(UShort width, UShort height) {
    viewport.width = width;
    viewport.height = height;
    viewportAPI.resize(&viewport, width, height);
}
