#include "stpch.h"

#ifdef ST_RENDERER_DX11
    #include "Sentinel/Graphics/Buffer/ConstantbufferAPI.h"
    #include "Sentinel/Graphics/Device/ContextAPI.h"

    #include "Platform/DirectX11/Graphics/Core/DX11Common.h"

namespace Sentinel {
    ConstantbufferData* Sentinel::ConstantbufferAPI::CreateConstantbufferData(
        PoolAllocator<ConstantbufferData>& allocator,
        ContextData* context,
        UInt32 size,
        UInt32 bindSlot,
        CBufferUsageType usageType) {
        ConstantbufferData* bufferObject = allocator.New();
        bufferObject->Context = context;
        Create(bufferObject, size, bindSlot, usageType);
        bufferObject->m_Size = size;
        bufferObject->m_UsageType = usageType;
        return bufferObject;
    }

    void ConstantbufferAPI::VSBind(ConstantbufferData* dataObject) {
        ContextAPI::GetNativeContext(dataObject->Context)
            ->VSSetConstantBuffers(dataObject->m_BindSlot, 1, &(dataObject->m_Buffer));
    }

    void ConstantbufferAPI::PSBind(ConstantbufferData* dataObject) {
        ContextAPI::GetNativeContext(dataObject->Context)
            ->PSSetConstantBuffers(dataObject->m_BindSlot, 1, &(dataObject->m_Buffer));
    }

    void ConstantbufferAPI::CSBind(ConstantbufferData* dataObject) {
        ContextAPI::GetNativeContext(dataObject->Context)
            ->CSSetConstantBuffers(dataObject->m_BindSlot, 1, &(dataObject->m_Buffer));
    }

    void ConstantbufferAPI::SetStaticData(ConstantbufferData* dataObject, void* data) {
        ContextAPI::GetNativeContext(dataObject->Context)->UpdateSubresource(dataObject->m_Buffer, 0, 0, data, 0, 0);
    }

    void ConstantbufferAPI::SetDynamicData(ConstantbufferData* dataObject, void* data) {
        D3D11_MAPPED_SUBRESOURCE subresource;
        SecureZeroMemory(&subresource, sizeof(subresource));
        ContextAPI::GetNativeContext(dataObject->Context)
            ->Map(dataObject->m_Buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &subresource);
        memcpy(subresource.pData, data, dataObject->m_Size);
        ContextAPI::GetNativeContext(dataObject->Context)->Unmap(dataObject->m_Buffer, 0);
    }

    void ConstantbufferAPI::Clean(ConstantbufferData* dataObject) {
        dataObject->m_Buffer->Release();
        dataObject->m_Buffer = 0;
    }

    void ConstantbufferAPI::Create(
        ConstantbufferData* dataObject, UInt32 size, UInt32 bindSlot, CBufferUsageType usageType) {
        D3D11_BUFFER_DESC description;
        SecureZeroMemory(&description, sizeof(description));
        description.ByteWidth = size;
        description.Usage = usageType == CBufferUsageType::DEFAULT ? D3D11_USAGE_DEFAULT : D3D11_USAGE_DYNAMIC;
        description.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
        description.CPUAccessFlags = usageType == CBufferUsageType::DYNAMIC ? D3D11_CPU_ACCESS_WRITE : 0;
        description.MiscFlags = 0;
        description.StructureByteStride = 0;

        ContextAPI::GetDevice(dataObject->Context)->CreateBuffer(&description, nullptr, &(dataObject->m_Buffer));
    }
}  // namespace Sentinel
#endif  // ST_RENDERER_DX11