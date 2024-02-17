#ifndef WINDOWS_GRAPHICS_D3D11_BUFFER_D3D11CONSTANTBUFFER_H
#define WINDOWS_GRAPHICS_D3D11_BUFFER_D3D11CONSTANTBUFFER_H

#ifdef _MSC_VER
    #pragma once
#endif  // _MSC_VER

typedef struct ConstantbufferData ConstantbufferData;
typedef enum ShaderType ShaderType;

void Sentinel_D3D11Constantbuffer_Create(ConstantbufferData* buffer);

void Sentinel_D3D11Constantbuffer_Destroy(ConstantbufferData* buffer);

void Sentinel_D3D11Constantbuffer_Bind(ConstantbufferData* buffer, ShaderType type);

void Sentinel_D3D11Constantbuffer_SetData(ConstantbufferData* buffer, void* data);

#endif  // !WINDOWS_GRAPHICS_D3D11_BUFFER_D3D11CONSTANTBUFFER_H
