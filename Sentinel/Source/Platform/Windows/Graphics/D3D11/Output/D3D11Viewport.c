#include "stpch.h"
#include "Graphics/D3D11/Device/D3D11Device.h"
#include "Graphics/D3D11/Output/D3D11Viewport.h"
#include "Graphics/Output/Viewport.h"

#define D3D11_NO_HELPERS
#define CINTERFACE
#define COBJMACROS

#include <d3d11.h>

#pragma warning(push)
#pragma warning(disable : 6011)

static D3D11_VIEWPORT nativeViewport;

void Sentinel_D3D11Viewport_Create(ViewportData* viewport) {
    nativeViewport.TopLeftX = viewport->topX;
    nativeViewport.TopLeftY = viewport->topY;
    nativeViewport.Width = viewport->width;
    nativeViewport.Height = viewport->height;
    nativeViewport.MinDepth = viewport->minDepth;
    nativeViewport.MaxDepth = viewport->maxDepth;
}

void Sentinel_D3D11Viewport_Bind(ViewportData* viewport) {
    ID3D11DeviceContext_RSSetViewports(Sentinel_D3D11Device_GetContext(), 1, &nativeViewport);
}

void Sentinel_D3D11Viewport_Resize(ViewportData* viewport, UShort width, UShort height) {
    nativeViewport.Width = width;
    nativeViewport.Height = height;
}

#pragma warning(pop)
