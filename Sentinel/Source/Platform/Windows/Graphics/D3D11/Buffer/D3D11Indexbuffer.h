#ifndef WINDOWS_GRAPHICS_D3D11_BUFFER_D3D11INDEXBUFFER_H
#define WINDOWS_GRAPHICS_D3D11_BUFFER_D3D11INDEXBUFFER_H

#ifdef _MSC_VER
    #pragma once
#endif  // _MSC_VER

typedef struct IndexbufferData IndexbufferData;

void Sentinel_D3D11Indexbuffer_Create(IndexbufferData* buffer);

void Sentinel_D3D11Indexbuffer_Destroy(IndexbufferData* buffer);

void Sentinel_D3D11Indexbuffer_Bind(IndexbufferData* buffer);

void Sentinel_D3D11Indexbuffer_Unbind(IndexbufferData* buffer);

#endif  // !WINDOWS_GRAPHICS_D3D11_BUFFER_D3D11INDEXBUFFER_H
