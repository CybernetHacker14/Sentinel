#include "stpch.h"

#include "Platform/DirectX11/Renderer/Components/Buffers/DirectX11IndexBuffer.h"

namespace Sentinel
{
	DirectX11IndexBuffer::DirectX11IndexBuffer(void* indices, uint32_t count)
		: m_Count(count) {
		D3D11_BUFFER_DESC description;
		ZeroMemory(&description, sizeof(description));

		description.Usage = D3D11_USAGE_DEFAULT;
		description.ByteWidth = sizeof(uint32_t) * count;
		description.BindFlags = D3D11_BIND_INDEX_BUFFER;
		description.CPUAccessFlags = 0;
		description.MiscFlags = 0;
		description.StructureByteStride = sizeof(uint32_t);

		D3D11_SUBRESOURCE_DATA subresource;
		ZeroMemory(&subresource, sizeof(subresource));
		subresource.pSysMem = indices;
		subresource.SysMemPitch = 0;
		subresource.SysMemSlicePitch = 0;

		DirectX11Internal::GetInternalHandle()->GetDevice()->CreateBuffer(
			&description, &subresource, &m_IndexBufferID);
	}

	DirectX11IndexBuffer::~DirectX11IndexBuffer() {
		m_IndexBufferID->Release();
	}

	void DirectX11IndexBuffer::Bind() const {
		DirectX11Internal::GetInternalHandle()->GetDeviceContext()->IASetIndexBuffer(
			m_IndexBufferID, DXGI_FORMAT_R32_UINT, 0);
	}

	void DirectX11IndexBuffer::Unbind() const {
		DirectX11Internal::GetInternalHandle()->GetDeviceContext()->IASetIndexBuffer(
			nullptr, DXGI_FORMAT_R32_UINT, 0);
	}
}
