#include "stpch.h"
#include "_EXPORT/Graphics/Texture/RenderTexture2DData_EXPORT.h"
#include "Allocator/FixedSlabAllocator.h"
#include "Graphics/D3D11/Device/D3D11Device.h"
#include "Graphics/D3D11/Texture/D3D11RenderTexture2D.h"
#include "Graphics/Device/Swapchain.h"
#include "Graphics/Texture/RenderTexture2D.h"

#define D3D11_NO_HELPERS
#define CINTERFACE
#define COBJMACROS

#include <d3d11.h>
#include <dxgi.h>

#pragma warning(push)
#pragma warning(disable : 6011)

static FixedSlabAllocator rT2DNativeAllocator;

void Sentinel_D3D11RenderTexture2D_Init() {
    Sentinel_FixedSlabAllocator_Allocate(&rT2DNativeAllocator, sizeof(RT2DD3D11Native), 32);
}

void Sentinel_D3D11RenderTexture2D_Deinit() {
    Sentinel_FixedSlabAllocator_Deallocate(&rT2DNativeAllocator);
}

void Sentinel_D3D11RenderTexture2D_Create(RenderTexture2DData* renderTexture) {
    // Let's hold on this one for a bit
}

void Sentinel_D3D11RenderTexture2D_CreateFromSwapchain(RenderTexture2DData* renderTexture, SwapchainData* swapchain) {
    if (renderTexture->native == NULL) {
        UShort index;
        renderTexture->native = Sentinel_FixedSlabAllocator_New(&rT2DNativeAllocator, &index);

    } else {
        ID3D11DeviceContext_OMSetRenderTargets(Sentinel_D3D11Device_GetContext(), 0, NULL, NULL);
        ID3D11RenderTargetView_Release(((RT2DD3D11Native*)renderTexture->native)->nativeRTV);
        ((RT2DD3D11Native*)renderTexture->native)->nativeRTV = NULL;
    }

    IDXGISwapChain* dxswapchain = (IDXGISwapChain*)(swapchain->native);
    IDXGISwapChain_GetBuffer(
        dxswapchain, 0, &IID_ID3D11Texture2D, (void**)&(((RT2DD3D11Native*)renderTexture->native)->nativeTex));

    ID3D11Device_CreateRenderTargetView(
        Sentinel_D3D11Device_GetDevice(),
        (ID3D11Resource*)(((RT2DD3D11Native*)renderTexture->native)->nativeTex),
        NULL,
        &(((RT2DD3D11Native*)renderTexture->native)->nativeRTV));
}

void Sentinel_D3D11RenderTexture2D_Destroy(RenderTexture2DData* renderTexture) {
    ID3D11RenderTargetView* rtv = ((RT2DD3D11Native*)renderTexture->native)->nativeRTV;
    ID3D11ShaderResourceView* srv = ((RT2DD3D11Native*)renderTexture->native)->nativeSRV;
    ID3D11Texture2D* tex = ((RT2DD3D11Native*)renderTexture->native)->nativeTex;

    if (rtv) ID3D11RenderTargetView_Release(rtv);
    if (srv) ID3D11ShaderResourceView_Release(srv);
    if (tex) ID3D11Texture2D_Release(tex);

    Sentinel_FixedSlabAllocator_Delete(&rT2DNativeAllocator, renderTexture->native);
}

void Sentinel_D3D11RenderTexture2D_BindRTV(RenderTexture2DData* renderTexture) {
    ID3D11RenderTargetView* rtv = ((RT2DD3D11Native*)renderTexture->native)->nativeRTV;
    ID3D11DeviceContext_OMSetRenderTargets(Sentinel_D3D11Device_GetContext(), 1, &rtv, NULL);
}

void Sentinel_D3D11RenderTexture2D_UnbindRTV(RenderTexture2DData* renderTexture) {
    ID3D11RenderTargetView* rtv = NULL;
    ID3D11DeviceContext_OMSetRenderTargets(Sentinel_D3D11Device_GetContext(), 1, &rtv, NULL);
}

void Sentinel_D3D11RenderTexture2D_Clear(RenderTexture2DData* renderTexture, float color[4]) {
    ID3D11RenderTargetView* rtv = ((RT2DD3D11Native*)renderTexture->native)->nativeRTV;
    ID3D11DeviceContext_ClearRenderTargetView(Sentinel_D3D11Device_GetContext(), rtv, color);
}

#pragma warning(pop)
