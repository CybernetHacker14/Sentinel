#include "stpch.h"

#if ST_RENDERER_DX11
    #include "Sentinel/Graphics/Buffer/VertexbufferAPI.h"
    #include "Sentinel/Graphics/Device/ContextAPI.h"

    #include "Platform/DirectX11/Graphics/Core/DX11Common.h"

namespace Sentinel {
    VertexbufferData* Sentinel::VertexbufferAPI::CreateVertexbufferData(
        PoolAllocator<VertexbufferData>& allocator, ContextData* context, UInt32 size) {
        VertexbufferData* bufferObject = allocator.New();
        bufferObject->Context = context;
        Create(bufferObject, size);
        return bufferObject;
    }

    VertexbufferData* VertexbufferAPI::CreateVertexbufferData(
        PoolAllocator<VertexbufferData>& allocator, ContextData* context, void* vertices, UInt32 size) {
        return nullptr;
    }

    void VertexbufferAPI::Bind(VertexbufferData* dataObject, UInt32 stride) {
    }

    void VertexbufferAPI::Unbind(VertexbufferData* dataObject) {
    }

    void VertexbufferAPI::SetData(VertexbufferData* dataObject, const void* vertices, UInt32 size) {
    }

    void VertexbufferAPI::Clean(VertexbufferData* dataObject) {
    }

    void VertexbufferAPI::Create(VertexbufferData* dataObject, UInt32 size) {
    }

    void VertexbufferAPI::Create(VertexbufferData* dataObject, void* vertices, UInt32 size) {
    }

    /*void DX11VertexbufferAPI::Bind(VertexbufferData* dataObject, UInt32 stride) {
        DX11VertexbufferData* buffer = VertexbufferAPI::Cast<DX11VertexbufferData>(dataObject);
        UInt32 offset = 0;
        UInt32 vStride = stride;
        DX11ContextData* dContext = ContextAPI::Cast<DX11ContextData>(dataObject->Context);
        DX11ContextAPI::GetNativeContext(dContext)->IASetVertexBuffers(
            0, 1, &(buffer->m_Buffer), &vStride, &offset);
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
            buffer->m_Buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &subresource);
        memcpy(subresource.pData, vertices, size);
        DX11ContextAPI::GetNativeContext(dContext)->Unmap(buffer->m_Buffer, 0);
    }

    void DX11VertexbufferAPI::Clean(VertexbufferData* dataObject) {
        DX11VertexbufferData* buffer = VertexbufferAPI::Cast<DX11VertexbufferData>(dataObject);
        buffer->m_Buffer = nullptr;
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
        DX11ContextAPI::GetDevice(dContext)->CreateBuffer(&description, nullptr, &(dataObject->m_Buffer));
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
        DX11ContextAPI::GetDevice(dContext)->CreateBuffer(&description, &subresource, &(dataObject->m_Buffer));
    }*/
}  // namespace Sentinel
#endif  // ST_RENDERER_DX11