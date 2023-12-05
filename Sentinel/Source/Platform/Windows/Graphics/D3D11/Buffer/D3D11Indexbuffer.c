#include "stpch.h"
#include "Graphics/Buffer/Indexbuffer.h"
#include "Graphics/D3D11/Buffer/D3D11Indexbuffer.h"
#include "Graphics/D3D11/Device/D3D11Device.h"
#include "Graphics/Geometry/Geometry.h"

#define D3D11_NO_HELPERS
#define CINTERFACE
#define COBJMACROS

#include <d3d11.h>

#pragma warning(push)
#pragma warning(disable : 6011)

void Sentinel_D3D11Indexbuffer_Create(IndexbufferData* buffer) {
    D3D11_BUFFER_DESC desc;
    SecureZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));
    desc.Usage = D3D11_USAGE_IMMUTABLE;
    desc.ByteWidth = buffer->geometry->indexSize * buffer->geometry->indexCount;
    desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
    desc.CPUAccessFlags = 0;
    desc.MiscFlags = 0;
    desc.StructureByteStride = buffer->geometry->indexSize;

    D3D11_SUBRESOURCE_DATA data;
    SecureZeroMemory(&data, sizeof(D3D11_SUBRESOURCE_DATA));
    data.pSysMem = buffer->geometry->indices;
    data.SysMemPitch = 0;
    data.SysMemSlicePitch = 0;

    ID3D11Buffer* iBuffer = NULL;
    ID3D11Device_CreateBuffer(Sentinel_D3D11Device_GetDevice(), &desc, &data, &iBuffer);
    buffer->native = iBuffer;
}

void Sentinel_D3D11Indexbuffer_Destroy(IndexbufferData* buffer) {
    ID3D11Buffer* iBuffer = (ID3D11Buffer*)(buffer->native);
    ID3D11Buffer_Release(iBuffer);
}

void Sentinel_D3D11Indexbuffer_Bind(IndexbufferData* buffer) {
    ID3D11Buffer* iBuffer = (ID3D11Buffer*)(buffer->native);
    ID3D11DeviceContext_IASetIndexBuffer(Sentinel_D3D11Device_GetContext(), iBuffer, DXGI_FORMAT_R32_UINT, 0);
}

void Sentinel_D3D11Indexbuffer_Unbind(IndexbufferData* buffer) {
    ID3D11DeviceContext_IASetIndexBuffer(Sentinel_D3D11Device_GetContext(), NULL, DXGI_FORMAT_R32_UINT, 0);
}

#pragma warning(pop)
