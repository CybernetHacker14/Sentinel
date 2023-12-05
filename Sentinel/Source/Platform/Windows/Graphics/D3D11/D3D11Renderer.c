#include "stpch.h"
#include "Graphics/D3D11/Buffer/D3D11VertexLayout.h"
#include "Graphics/D3D11/D3D11Renderer.h"
#include "Graphics/D3D11/Device/D3D11Device.h"

// https://gist.github.com/msmshazan/dfd5362004be37ff5e016b6a42be5083

#pragma warning(push)
#pragma warning(disable : 6011)

/*
static void RenderFrame() {
    // clear background

    ID3D11DeviceContext_Draw(context, _countof(vertices), 0);
}

static void RenderResize(int width, int height) {
    if (windowRT) {
        ID3D11DeviceContext_OMSetRenderTargets(context, 0, NULL, NULL);
        ID3D11RenderTargetView_Release(windowRT);
        windowRT = NULL;
    }

    IDXGISwapChain_ResizeBuffers(swapchain, 0, width, height, DXGI_FORMAT_UNKNOWN, 0);
    ID3D11Texture2D* windowBuffer;
    IDXGISwapChain_GetBuffer(swapchain, 0, &IID_ID3D11Texture2D, (void**)&windowBuffer);

    ID3D11Device_CreateRenderTargetView(device, (ID3D11Resource*)windowBuffer, NULL, &windowRT);
    ID3D11Texture2D_Release(windowBuffer);

}
*/

/*
void RendererD3D11_Init() {
RenderResize((int)(Sentinel_Window_GetWidth()), (int)(Sentinel_Window_GetHeight()));

{
    ID3D11DeviceContext_OMSetRenderTargets(context, 1, &windowRT, NULL);

    const UINT stride = sizeof(struct Vertex);
    const UINT offset = 0;
    ID3D11DeviceContext_IASetVertexBuffers(context, 0, 1, &vertexBuffer, &stride, &offset);
}
}
*/

void Sentinel_D3D11Renderer_Init() {
    Sentinel_D3D11Device_Init();
}

void Sentinel_D3D11Renderer_PreUpdateInit() {
    Sentinel_D3D11VertexLayout_Bind();
}

void Sentinel_D3D11Renderer_OnUpdate() {
}

void Sentinel_D3D11Renderer_OnRender() {
    Sentinel_D3D11Device_Draw();
}

void Sentinel_D3D11Renderer_Deinit() {
    Sentinel_D3D11VertexLayout_Deinit();
    Sentinel_D3D11Device_Deinit();
}

#pragma warning(pop)
