#include "stpch.h"
#include "Platform/DirectX11/Graphics/Core/DX11Common.h"
#include "Platform/DirectX11/Graphics/Components/RenderResources/Buffers/DX11Vertexbuffer.h"

namespace Sentinel
{
	DX11Vertexbuffer::DX11Vertexbuffer(UInt32 size) {

		m_BindFunction = ST_BIND_EVENT_FN(Bind);
		m_UnbindFunction = ST_BIND_EVENT_FN(Unbind);
		m_SetDataFunction = ST_BIND_EVENT_FN(SetData);
		m_CleanFunction = ST_BIND_EVENT_FN(Clean);

		D3D11_BUFFER_DESC description;
		SecureZeroMemory(&description, sizeof(description));

		description.Usage = D3D11_USAGE_DYNAMIC;
		description.ByteWidth = size;
		description.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		description.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		description.MiscFlags = 0;
		description.StructureByteStride = 0;
		DX11Common::GetDevice()->CreateBuffer(&description, nullptr, &m_VertexbufferPtr);
	}

	DX11Vertexbuffer::DX11Vertexbuffer(void* verticesData, UInt32 size) {

		m_BindFunction = ST_BIND_EVENT_FN(Bind);
		m_UnbindFunction = ST_BIND_EVENT_FN(Unbind);
		m_SetDataFunction = ST_BIND_EVENT_FN(SetData);
		m_CleanFunction = ST_BIND_EVENT_FN(Clean);

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
		subresource.pSysMem = verticesData;
		subresource.SysMemPitch = 0;
		subresource.SysMemSlicePitch = 0;

		DX11Common::GetDevice()->CreateBuffer(&description, &subresource, &m_VertexbufferPtr);
	}

	void DX11Vertexbuffer::Bind(UInt32 stride) const {
		UInt32 offset = 0;
		DX11Common::GetContext()->IASetVertexBuffers(0, 1, &m_VertexbufferPtr, &stride, &offset);
	}

	void DX11Vertexbuffer::Unbind() const {
		// Expects an array of ID3D11Buffer*, Direct nullptr clearly isn't
		ID3D11Buffer* nullBuffer = nullptr;
		UInt32 nullOffset = 0;
		UInt32 nullStride = 0;
		DX11Common::GetContext()->IASetVertexBuffers(0, 1, &nullBuffer, &nullStride, &nullOffset);
	}

	void DX11Vertexbuffer::SetData(const void* vertices, UInt32 size) {
		D3D11_MAPPED_SUBRESOURCE subresource;
		DX11Common::GetContext()->Map(m_VertexbufferPtr, 0, D3D11_MAP_WRITE_DISCARD, 0, &subresource);
		memcpy(subresource.pData, vertices, size);
		DX11Common::GetContext()->Unmap(m_VertexbufferPtr, 0);
	}

	void DX11Vertexbuffer::Clean() {
		m_VertexbufferPtr = nullptr;
	}
}
