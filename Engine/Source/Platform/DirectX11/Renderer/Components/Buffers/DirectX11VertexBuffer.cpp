#include "stpch.h"

#include "Platform/DirectX11/Renderer/Components/Buffers/DirectX11VertexBuffer.h"

namespace Sentinel
{
	DirectX11VertexBuffer::DirectX11VertexBuffer(uint32_t size) {
		D3D11_BUFFER_DESC description;
		ZeroMemory(&description, sizeof(description));

		description.Usage = D3D11_USAGE_DYNAMIC;
		description.ByteWidth = size;
		description.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		description.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		description.MiscFlags = 0;
		description.StructureByteStride = m_Layout.GetStride();

		DirectX11Internal::GetInternalHandle()->GetDevice()->CreateBuffer(
			&description, nullptr, &m_VertexBufferID);
	}

	DirectX11VertexBuffer::DirectX11VertexBuffer(void* vertices, uint32_t size) {
		D3D11_BUFFER_DESC description;
		ZeroMemory(&description, sizeof(description));

		description.Usage = D3D11_USAGE_DEFAULT;
		description.ByteWidth = size;
		description.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		description.CPUAccessFlags = 0;
		description.MiscFlags = 0;
		description.StructureByteStride = m_Layout.GetStride();

		D3D11_SUBRESOURCE_DATA subresource;
		ZeroMemory(&subresource, sizeof(subresource));
		subresource.pSysMem = vertices;
		subresource.SysMemPitch = 0;
		subresource.SysMemSlicePitch = 0;

		DirectX11Internal::GetInternalHandle()->GetDevice()->CreateBuffer(
			&description, &subresource, &m_VertexBufferID);
	}

	DirectX11VertexBuffer::~DirectX11VertexBuffer() {
		m_VertexBufferID->Release();
	}

	void DirectX11VertexBuffer::Bind() const {
		uint32_t stride = m_Layout.GetStride();
		uint32_t offset = 0;
		DirectX11Internal::GetInternalHandle()->GetDeviceContext()->IASetVertexBuffers(
			0, 1, &m_VertexBufferID, &stride, &offset);
	}

	void DirectX11VertexBuffer::Unbind() const {
		DirectX11Internal::GetInternalHandle()->GetDeviceContext()->IASetVertexBuffers(
			0, 1, nullptr, nullptr, nullptr);
	}

	void DirectX11VertexBuffer::SetData(const void* data, uint32_t size) {
		this->Bind();

		auto deviceContext = DirectX11Internal::GetInternalHandle()->GetDeviceContext();
		D3D11_MAPPED_SUBRESOURCE subresource;
		deviceContext->Map(m_VertexBufferID, 0, D3D11_MAP_WRITE_DISCARD, 0, &subresource);
		memcpy(subresource.pData, data, size);
		deviceContext->Unmap(m_VertexBufferID, 0);

		this->Bind();
	}
}
