#include "stpch.h"
#include "Graphics/Buffer/Vertexbuffer.h"
#include "Graphics/D3D11/Buffer/D3D11Vertexbuffer.h"
#include "Graphics/D3D11/Device/D3D11Device.h"
#include "Graphics/Geometry/Geometry.h"

#define D3D11_NO_HELPERS
#define CINTERFACE
#define COBJMACROS

#include <d3d11.h>

#pragma warning(push)
#pragma warning(disable : 6011)

void Sentinel_D3D11Vertexbuffer_Create(VertexbufferData* buffer) {
    D3D11_BUFFER_DESC desc;
    SecureZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));
    desc.Usage = D3D11_USAGE_IMMUTABLE;
    desc.ByteWidth = buffer->geometry->vertexSize * buffer->geometry->vertexCount;
    desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    desc.CPUAccessFlags = 0;
    desc.MiscFlags = 0;
    desc.StructureByteStride = 0;

    D3D11_SUBRESOURCE_DATA data;
    SecureZeroMemory(&data, sizeof(D3D11_SUBRESOURCE_DATA));
    data.pSysMem = buffer->geometry->vertices;
    data.SysMemPitch = 0;
    data.SysMemSlicePitch = 0;

    ID3D11Buffer* vBuffer = NULL;
    ID3D11Device_CreateBuffer(Sentinel_D3D11Device_GetDevice(), &desc, &data, &vBuffer);
    buffer->native = vBuffer;
}

void Sentinel_D3D11Vertexbuffer_Destroy(VertexbufferData* buffer) {
    ID3D11Buffer* vBuffer = (ID3D11Buffer*)(buffer->native);
    ID3D11Buffer_Release(vBuffer);
}

void Sentinel_D3D11Vertexbuffer_Bind(VertexbufferData* buffer) {
    ID3D11Buffer* vBuffer = (ID3D11Buffer*)(buffer->native);
    const UINT stride = buffer->geometry->vertexSize;
    const UINT offset = 0;
    ID3D11DeviceContext_IASetVertexBuffers(Sentinel_D3D11Device_GetContext(), 0, 1, &vBuffer, &stride, &offset);
}

void Sentinel_D3D11Vertexbuffer_Unbind(VertexbufferData* buffer) {
    ID3D11Buffer* nullBuffer = NULL;
    UInt nullOffset = 0;
    UInt nullStride = 0;
    ID3D11DeviceContext_IASetVertexBuffers(
        Sentinel_D3D11Device_GetContext(), 0, 1, &nullBuffer, &nullStride, &nullOffset);
}

#pragma warning(pop)
