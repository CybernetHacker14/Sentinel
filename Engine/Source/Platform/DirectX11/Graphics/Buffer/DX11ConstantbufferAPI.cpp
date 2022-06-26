#include "stpch.h"
#include "Platform/DirectX11/Graphics/Core/DX11Common.h"
#include "Platform/DirectX11/Graphics/Buffer/DX11ConstantbufferAPI.h"
#include "Platform/DirectX11/Graphics/Buffer/DX11ConstantbufferData.h"

namespace Sentinel {
    DX11ConstantbufferAPI::_init DX11ConstantbufferAPI::_initializer;

    void DX11ConstantbufferAPI::VSBind(ConstantbufferData* dataObject) {
        DX11ConstantbufferData* buffer = ConstantbufferAPI::Cast<DX11ConstantbufferData>(dataObject);
        DX11Common::GetContext()->VSSetConstantBuffers(buffer->m_BindSlot, 1, &(buffer->m_Buffer));
    }

    void DX11ConstantbufferAPI::PSBind(ConstantbufferData* dataObject) {
        DX11ConstantbufferData* buffer = ConstantbufferAPI::Cast<DX11ConstantbufferData>(dataObject);
        DX11Common::GetContext()->PSSetConstantBuffers(buffer->m_BindSlot, 1, &(buffer->m_Buffer));
    }

    void DX11ConstantbufferAPI::CSBind(ConstantbufferData* dataObject) {
        DX11ConstantbufferData* buffer = ConstantbufferAPI::Cast<DX11ConstantbufferData>(dataObject);
        DX11Common::GetContext()->CSSetConstantBuffers(buffer->m_BindSlot, 1, &(buffer->m_Buffer));
    }

    void DX11ConstantbufferAPI::SetStaticData(ConstantbufferData* dataObject, void* data) {
        DX11ConstantbufferData* buffer = ConstantbufferAPI::Cast<DX11ConstantbufferData>(dataObject);
        DX11Common::GetContext()->UpdateSubresource(buffer->m_Buffer, 0, 0, data, 0, 0);
    }

    void DX11ConstantbufferAPI::SetDynamicData(ConstantbufferData* dataObject, void* data) {
        DX11ConstantbufferData* buffer = ConstantbufferAPI::Cast<DX11ConstantbufferData>(dataObject);
        D3D11_MAPPED_SUBRESOURCE subresource;
        SecureZeroMemory(&subresource, sizeof(subresource));
        DX11Common::GetContext()->Map(buffer->m_Buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &subresource);
        memcpy(subresource.pData, data, buffer->m_Size);
        DX11Common::GetContext()->Unmap(buffer->m_Buffer, 0);
    }

    void DX11ConstantbufferAPI::Clean(ConstantbufferData* dataObject) {
        DX11ConstantbufferData* buffer = ConstantbufferAPI::Cast<DX11ConstantbufferData>(dataObject);
        buffer->m_Buffer->Release();
    }

    void DX11ConstantbufferAPI::CreateNative(
        DX11ConstantbufferData* dataObject, UInt32 size, UInt32 bindSlot, CBufferUsageType usageType) {
        D3D11_BUFFER_DESC description;
        SecureZeroMemory(&description, sizeof(description));
        description.ByteWidth = size;
        description.Usage = usageType == CBufferUsageType::DEFAULT ? D3D11_USAGE_DEFAULT : D3D11_USAGE_DYNAMIC;
        description.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
        description.CPUAccessFlags = usageType == CBufferUsageType::DYNAMIC ? D3D11_CPU_ACCESS_WRITE : 0;
        description.MiscFlags = 0;
        description.StructureByteStride = 0;

        DX11Common::GetDevice()->CreateBuffer(&description, nullptr, &(dataObject->m_Buffer));
    }
}  // namespace Sentinel