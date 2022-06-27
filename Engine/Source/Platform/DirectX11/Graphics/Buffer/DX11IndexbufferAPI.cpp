#include "stpch.h"
#include "Platform/DirectX11/Graphics/Core/DX11Common.h"
#include "Platform/DirectX11/Graphics/Buffer/DX11IndexbufferAPI.h"
#include "Platform/DirectX11/Graphics/Buffer/DX11IndexbufferData.h"

namespace Sentinel {
    DX11IndexbufferAPI::_init DX11IndexbufferAPI::_initializer;

    void DX11IndexbufferAPI::Bind(IndexbufferData* dataObject) {
        DX11IndexbufferData* buffer = IndexbufferAPI::Cast<DX11IndexbufferData>(dataObject);
        DX11Common::GetContext()->IASetIndexBuffer(buffer->m_IndexbufferPtr, DXGI_FORMAT_R32_UINT, 0);
    }

    void DX11IndexbufferAPI::Unbind(IndexbufferData* dataObject) {
        DX11Common::GetContext()->IASetIndexBuffer(nullptr, DXGI_FORMAT_R32_UINT, 0);
    }

    void DX11IndexbufferAPI::Clean(IndexbufferData* dataObject) {
        DX11IndexbufferData* buffer = IndexbufferAPI::Cast<DX11IndexbufferData>(dataObject);
        buffer->m_IndexbufferPtr->Release();
    }

    void DX11IndexbufferAPI::CreateNative(DX11IndexbufferData* dataObject, void* indices, UInt32 count) {
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

        DX11Common::GetDevice()->CreateBuffer(&description, &subresource, &(dataObject->m_IndexbufferPtr));
    }
}  // namespace Sentinel