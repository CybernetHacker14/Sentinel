#ifndef WINDOWS_GRAPHICS_D3D11_D3D11RENDERER_H
#define WINDOWS_GRAPHICS_D3D11_D3D11RENDERER_H

#ifdef _MSC_VER
    #pragma once
#endif  // _MSC_VER

void Sentinel_D3D11Renderer_Init();

void Sentinel_D3D11Renderer_PreUpdateInit();

void Sentinel_D3D11Renderer_OnUpdate();

void Sentinel_D3D11Renderer_OnRender();

void Sentinel_D3D11Renderer_Deinit();

#endif  // !WINDOWS_GRAPHICS_D3D11_D3D11RENDERER_H
