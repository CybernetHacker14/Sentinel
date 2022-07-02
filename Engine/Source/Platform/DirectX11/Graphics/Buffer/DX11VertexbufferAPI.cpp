#include "stpch.h"
#include "Platform/DirectX11/Graphics/Core/DX11Common.h"
#include "Platform/DirectX11/Graphics/Buffer/DX11VertexbufferAPI.h"
#include "Platform/DirectX11/Graphics/Buffer/DX11VertexbufferData.h"

#include "Sentinel/Graphics/Buffer/VertexbufferData.h"

#include "Platform/DirectX11/Graphics/Device/DX11ContextData.h"
#include "Platform/DirectX11/Graphics/Device/DX11ContextAPI.h"

namespace Sentinel {
    DX11VertexbufferAPI::_init DX11VertexbufferAPI::_initializer;

    void DX11VertexbufferAPI::Bind(VertexbufferData* dataObject, UInt32 stride) {
        DX11VertexbufferData* buffer = VertexbufferAPI::Cast<DX11VertexbufferData>(dataObject);
        UInt32 offset = 0;
        DX11ContextData* dContext = ContextAPI::Cast<DX11ContextData>(dataObject->Context);
        DX11ContextAPI::GetNativeContext(dContext)->IASetVertexBuffers(
            0, 1, &(buffer->m_VertexbufferPtr), &stride, &offset);
    }

    void DX11VertexbufferAPI::Unbind(VertexbufferData* dataObject) {
        // Expects an array of ID3D11Buffer*, Direct nullptr clearly isn't
        ID3D11Buffer* nullBuffer = nullptr;
        UInt32 nullOffset = 0;
        UInt32 nullStride = 0;
        DX11ContextData* dContext = ContextAPI::Cast<DX11ContextData>(dataObject->Context);
        DX11ContextAPI::GetNativeContext(dContext)->IASetVertexBuffers(0, 1, &nullBuffer, &nullStride, &nullOffset);
    }

    void DX11VertexbufferAPI::SetData(VertexbufferData* dataObject, const void* vertices, UInt32 size) {
        DX11VertexbufferData* buffer = VertexbufferAPI::Cast<DX11VertexbufferData>(dataObject);
        D3D11_MAPPED_SUBRESOURCE subresource;
        DX11ContextData* dContext = ContextAPI::Cast<DX11ContextData>(dataObject->Context);
        DX11ContextAPI::GetNativeContext(dContext)->Map(
            buffer->m_VertexbufferPtr, 0, D3D11_MAP_WRITE_DISCARD, 0, &subresource);
        memcpy(subresource.pData, vertices, size);
        DX11ContextAPI::GetNativeContext(dContext)->Unmap(buffer->m_VertexbufferPtr, 0);
    }

    void DX11VertexbufferAPI::Clean(VertexbufferData* dataObject) {
        DX11VertexbufferData* buffer = VertexbufferAPI::Cast<DX11VertexbufferData>(dataObject);
        buffer->m_VertexbufferPtr = nullptr;
    }

    void DX11VertexbufferAPI::CreateNative(DX11VertexbufferData* dataObject, UInt32 size) {
        D3D11_BUFFER_DESC description;
        SecureZeroMemory(&description, sizeof(description));

        description.Usage = D3D11_USAGE_DYNAMIC;
        description.ByteWidth = size;
        description.BindFlags = D3D11_BIND_VERTEX_BUFFER;
        description.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
        description.MiscFlags = 0;
        description.StructureByteStride = 0;
        DX11ContextData* dContext = ContextAPI::Cast<DX11ContextData>(dataObject->Context);
        DX11ContextAPI::GetDevice(dContext)->CreateBuffer(&description, nullptr, &(dataObject->m_VertexbufferPtr));
    }

    void DX11VertexbufferAPI::CreateNative(DX11VertexbufferData* dataObject, void* vertices, UInt32 size) {
        D3D11_BUFFER_DESC description;
        SecureZeroMemory(&description, sizeof(description));

        description.Usage = D3D11_USAGE_DEFAULT;
        description.ByteWidth = size;
        description.BindFlags = D3D11_BIND_VERTEX_BUFFER;
        description.CPUAccessFlags = 0;
        description.MiscFlags = 0;
        description.StructureByteStride = 0;

        D3D11_SUBRESOURCE_DATA subresource;
        SecureZeroMemory(&subresource, sizeof(subresource));
        subresource.pSysMem = vertices;
        subresource.SysMemPitch = 0;
        subresource.SysMemSlicePitch = 0;

        DX11ContextData* dContext = ContextAPI::Cast<DX11ContextData>(dataObject->Context);
        DX11ContextAPI::GetDevice(dContext)->CreateBuffer(&description, &subresource, &(dataObject->m_VertexbufferPtr));
    }
}  // namespace Sentinel