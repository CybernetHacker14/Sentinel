#ifndef WINDOWS_GRAPHICS_D3D11_BUFFER_D3D11VERTEXBUFFER_H
#define WINDOWS_GRAPHICS_D3D11_BUFFER_D3D11VERTEXBUFFER_H

#ifdef _MSC_VER
    #pragma once
#endif  // _MSC_VER

typedef struct VertexbufferData VertexbufferData;

void Sentinel_D3D11Vertexbuffer_Create(VertexbufferData* buffer);

void Sentinel_D3D11Vertexbuffer_Destroy(VertexbufferData* buffer);

void Sentinel_D3D11Vertexbuffer_Bind(VertexbufferData* buffer);

void Sentinel_D3D11Vertexbuffer_Unbind(VertexbufferData* buffer);

#endif  // !WINDOWS_GRAPHICS_D3D11_BUFFER_D3D11VERTEXBUFFER_H
