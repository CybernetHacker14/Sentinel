#ifndef WINDOWS_GRAPHICS_D3D11_D3D11RENDERTEXTURE2D_H
#define WINDOWS_GRAPHICS_D3D11_D3D11RENDERTEXTURE2D_H

#ifdef _MSC_VER
    #pragma once
#endif  // _MSC_VER

typedef struct RenderTexture2DData RenderTexture2DData;
typedef struct SwapchainData SwapchainData;

typedef struct ID3D11Texture2D ID3D11Texture2D;
typedef struct ID3D11RenderTargetView ID3D11RenderTargetView;
typedef struct ID3D11ShaderResourceView ID3D11ShaderResourceView;

typedef struct RT2DD3D11Native {
    ID3D11Texture2D* nativeTex;
    ID3D11RenderTargetView* nativeRTV;
    ID3D11ShaderResourceView* nativeSRV;
} RT2DD3D11Native;

void Sentinel_D3D11RenderTexture2D_Init();

void Sentinel_D3D11RenderTexture2D_Deinit();

void Sentinel_D3D11RenderTexture2D_Create(RenderTexture2DData* renderTexture);

void Sentinel_D3D11RenderTexture2D_CreateFromSwapchain(RenderTexture2DData* renderTexture, SwapchainData* swapchain);

void Sentinel_D3D11RenderTexture2D_Destroy(RenderTexture2DData* renderTexture);

void Sentinel_D3D11RenderTexture2D_BindRTV(RenderTexture2DData* renderTexture);

void Sentinel_D3D11RenderTexture2D_UnbindRTV(RenderTexture2DData* renderTexture);

void Sentinel_D3D11RenderTexture2D_Clear(RenderTexture2DData* renderTexture, float color[4]);

#endif  // !WINDOWS_GRAPHICS_D3D11_D3D11RENDERTEXTURE2D_H
