#include "stpch.h"

#ifdef ST_RENDERER_DX11
    #include "Sentinel/Graphics/Buffer/IndexbufferAPI.h"
    #include "Sentinel/Graphics/Device/ContextAPI.h"

    #include "Platform/DirectX11/Graphics/Core/DX11Common.h"

namespace Sentinel {
    IndexbufferData* Sentinel::IndexbufferAPI::CreateIndexbufferData(
        PoolAllocator<IndexbufferData>& allocator, ContextData* context, void* indices, UInt32 count) {
        IndexbufferData* bufferObject = allocator.New();
        bufferObject->Context = context;
        Create(bufferObject, indices, count);
        bufferObject->m_Count = count;
        return bufferObject;
    }

    void IndexbufferAPI::Bind(IndexbufferData* dataObject) {
        ContextAPI::GetNativeContext(dataObject->Context)
            ->IASetIndexBuffer(dataObject->m_Buffer, DXGI_FORMAT_R32_UINT, 0);
    }

    void IndexbufferAPI::Unbind(IndexbufferData* dataObject) {
        ContextAPI::GetNativeContext(dataObject->Context)->IASetIndexBuffer(nullptr, DXGI_FORMAT_R32_UINT, 0);
    }

    void IndexbufferAPI::Clean(IndexbufferData* dataObject) {
        dataObject->m_Buffer->Release();
    }

    void IndexbufferAPI::Create(IndexbufferData* dataObject, void* indices, UInt32 count) {
        D3D11_BUFFER_DESC description;
        SecureZeroMemory(&description, sizeof(description));

        description.Usage = D3D11_USAGE_DEFAULT;
        description.ByteWidth = sizeof(UInt32) * count;
        description.BindFlags = D3D11_BIND_INDEX_BUFFER;
        description.CPUAccessFlags = 0;
        description.MiscFlags = 0;
        description.StructureByteStride = sizeof(UInt32);

        D3D11_SUBRESOURCE_DATA subresource;
        SecureZeroMemory(&subresource, sizeof(subresource));
        subresource.pSysMem = indices;
        subresource.SysMemPitch = 0;
        subresource.SysMemSlicePitch = 0;

        ContextAPI::GetDevice(dataObject->Context)->CreateBuffer(&description, &subresource, &(dataObject->m_Buffer));
    }
}  // namespace Sentinel
#endif  // ST_RENDERER_DX11