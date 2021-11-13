#include "stpch.h"

#include "Platform/DirectX11/Graphics/Components/Buffers/DX11Indexbuffer.h"

namespace Sentinel
{
	DX11Indexbuffer::DX11Indexbuffer(void* indices, UInt32 count)
		:m_Count(count) {
		D3D11_BUFFER_DESC description;
		ZeroMemory(&description, sizeof(description));

		description.Usage = D3D11_USAGE_DEFAULT;
		description.ByteWidth = sizeof(UInt32) * count;
		description.BindFlags = D3D11_BIND_INDEX_BUFFER;
		description.CPUAccessFlags = 0;
		description.MiscFlags = 0;
		description.StructureByteStride = sizeof(UInt32);

		D3D11_SUBRESOURCE_DATA subresource;
		ZeroMemory(&subresource, sizeof(subresource));
		subresource.pSysMem = indices;
		subresource.SysMemPitch = 0;
		subresource.SysMemSlicePitch = 0;

		DX11Common::GetDevice()->CreateBuffer(&description, &subresource, &m_IndexbufferPtr);
	}

	DX11Indexbuffer::~DX11Indexbuffer() {
		m_IndexbufferPtr->Release();
	}

	void DX11Indexbuffer::Bind() const {
		DX11Common::GetContext()->IASetIndexBuffer(m_IndexbufferPtr, DXGI_FORMAT_R32_UINT, 0);
	}

	void DX11Indexbuffer::Unbind() const {
		DX11Common::GetContext()->IASetIndexBuffer(nullptr, DXGI_FORMAT_R32_UINT, 0);
	}
}
