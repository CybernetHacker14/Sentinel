#ifndef WINDOWS_GRAPHICS_D3D11_BUFFER_D3D11VERTEXLAYOUT_H
#define WINDOWS_GRAPHICS_D3D11_BUFFER_D3D11VERTEXLAYOUT_H

#ifdef _MSC_VER
    #pragma once
#endif  // _MSC_VER

typedef struct ID3D10Blob ID3D10Blob;

void Sentinel_D3D11VertexLayout_Init(ID3D10Blob* vertexShaderbytecode);

void Sentinel_D3D11VertexLayout_Deinit();

void Sentinel_D3D11VertexLayout_Bind();

#endif  // !WINDOWS_GRAPHICS_D3D11_BUFFER_D3D11VERTEXLAYOUT_H
