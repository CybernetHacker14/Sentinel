#ifndef WINDOWS_GRAPHICS_D3D11_DEVICE_D3D11DEVICE_H
#define WINDOWS_GRAPHICS_D3D11_DEVICE_D3D11DEVICE_H

#ifdef _MSC_VER
    #pragma once
#endif  // _MSC_VER

typedef struct ID3D11Device ID3D11Device;
typedef struct ID3D11DeviceContext ID3D11DeviceContext;
typedef struct IDXGIFactory IDXGIFactory;

void Sentinel_D3D11Device_Init();

void Sentinel_D3D11Device_Deinit();

void Sentinel_D3D11Device_Draw();

ID3D11Device* Sentinel_D3D11Device_GetDevice();

ID3D11DeviceContext* Sentinel_D3D11Device_GetContext();

IDXGIFactory* Sentinel_D3D11Device_GetFactory();

#endif  // !WINDOWS_GRAPHICS_D3D11_DEVICE_D3D11DEVICE_H
