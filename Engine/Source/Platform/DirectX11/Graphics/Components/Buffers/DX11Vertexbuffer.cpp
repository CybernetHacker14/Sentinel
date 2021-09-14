#include "stpch.h"

#include "Platform/DirectX11/Graphics/Components/Buffers/DX11Vertexbuffer.h"

namespace Sentinel
{
	DX11Vertexbuffer::DX11Vertexbuffer(UInt size) {
		D3D11_BUFFER_DESC description;
		ZeroMemory(&description, sizeof(description));

		description.Usage = D3D11_USAGE_DYNAMIC;
		description.ByteWidth = size;
		description.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		description.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		description.MiscFlags = 0;
		description.StructureByteStride = 0;
		DX11Common::GetDevice()->CreateBuffer(&description, nullptr, &m_VertexbufferPtr);
	}

	DX11Vertexbuffer::DX11Vertexbuffer(void* verticesData, UInt size) {
		D3D11_BUFFER_DESC description;
		ZeroMemory(&description, sizeof(description));

		description.Usage = D3D11_USAGE_DEFAULT;
		description.ByteWidth = size;
		description.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		description.CPUAccessFlags = 0;
		description.MiscFlags = 0;
		description.StructureByteStride = 0;

		D3D11_SUBRESOURCE_DATA subresource;
		ZeroMemory(&subresource, sizeof(subresource));
		subresource.pSysMem = verticesData;
		subresource.SysMemPitch = 0;
		subresource.SysMemSlicePitch = 0;

		DX11Common::GetDevice()->CreateBuffer(&description, &subresource, &m_VertexbufferPtr);
	}

	DX11Vertexbuffer::~DX11Vertexbuffer() {
		m_VertexbufferPtr->Release();
	}

	void DX11Vertexbuffer::Bind(UInt stride) const {
		UInt offset = 0;
		DX11Common::GetContext()->IASetVertexBuffers(0, 1, &m_VertexbufferPtr, &stride, &offset);
	}

	void DX11Vertexbuffer::Unbind() const {
		DX11Common::GetContext()->IASetVertexBuffers(0, 1, nullptr, nullptr, nullptr);
	}

	void DX11Vertexbuffer::SetData(const void* vertices, UInt size) {
		D3D11_MAPPED_SUBRESOURCE subresource;
		DX11Common::GetContext()->Map(m_VertexbufferPtr, 0, D3D11_MAP_WRITE_DISCARD, 0, &subresource);
		memcpy(subresource.pData, vertices, size);
		DX11Common::GetContext()->Unmap(m_VertexbufferPtr, 0);
	}
}
