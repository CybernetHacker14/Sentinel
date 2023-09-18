#include "stpch.h"

#ifdef ST_RENDERER_DX11
    #include "Sentinel/Graphics/Buffer/VertexbufferAPI.h"
    #include "Sentinel/Graphics/Device/ContextAPI.h"

    #include "Platform/Windows/DirectX11/Graphics/Core/DX11Common.h"

namespace Sentinel {
    VertexbufferData* VertexbufferAPI::CreateVertexbufferData(
        FixedSlabAllocator<VertexbufferData>& allocator, ContextData* context, UInt32 size) {
        VertexbufferData* bufferObject = allocator.New();
        bufferObject->Context = context;
        Create(bufferObject, size);
        return bufferObject;
    }

    VertexbufferData* VertexbufferAPI::CreateVertexbufferData(
        FixedSlabAllocator<VertexbufferData>& allocator, ContextData* context, void* vertices, UInt32 size) {
        VertexbufferData* bufferObject = allocator.New();
        bufferObject->m_Vertices = vertices;
        bufferObject->Context = context;
        Create(bufferObject, vertices, size);
        return bufferObject;
    }

    void VertexbufferAPI::Bind(VertexbufferData* dataObject, UInt32 stride) {
        UInt32 offset = 0;
        UInt32 vStride = stride;
        ContextAPI::GetNativeContext(dataObject->Context)
            ->IASetVertexBuffers(0, 1, &(dataObject->m_Buffer), &vStride, &offset);
    }

    void VertexbufferAPI::Unbind(VertexbufferData* dataObject) {
        ID3D11Buffer* nullBuffer = nullptr;
        UInt32 nullOffset = 0;
        UInt32 nullStride = 0;
        ContextAPI::GetNativeContext(dataObject->Context)
            ->IASetVertexBuffers(0, 1, &nullBuffer, &nullStride, &nullOffset);
    }

    void VertexbufferAPI::SetData(VertexbufferData* dataObject, const void* vertices, UInt32 size) {
        D3D11_MAPPED_SUBRESOURCE subresource;
        ContextAPI::GetNativeContext(dataObject->Context)
            ->Map(dataObject->m_Buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &subresource);
        memcpy(subresource.pData, vertices, size);
        ContextAPI::GetNativeContext(dataObject->Context)->Unmap(dataObject->m_Buffer, 0);
    }

    void VertexbufferAPI::Clean(VertexbufferData* dataObject) {
        dataObject->m_Buffer->Release();
        dataObject->m_Buffer = 0;
    }

    void VertexbufferAPI::Create(VertexbufferData* dataObject, UInt32 size) {
        D3D11_BUFFER_DESC description;
        SecureZeroMemory(&description, sizeof(description));

        description.Usage = D3D11_USAGE_DYNAMIC;
        description.ByteWidth = size;
        description.BindFlags = D3D11_BIND_VERTEX_BUFFER;
        description.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
        description.MiscFlags = 0;
        description.StructureByteStride = 0;
        ContextAPI::GetDevice(dataObject->Context)->CreateBuffer(&description, nullptr, &(dataObject->m_Buffer));
    }

    void VertexbufferAPI::Create(VertexbufferData* dataObject, void* vertices, UInt32 size) {
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

        ContextAPI::GetDevice(dataObject->Context)->CreateBuffer(&description, &subresource, &(dataObject->m_Buffer));
    }
}  // namespace Sentinel
#endif  // ST_RENDERER_DX11