#include "stpch.h"
#include "Platform/DirectX11/Graphics/Components/Buffers/DX11Indexbuffer.h"

namespace Sentinel
{
	DX11Indexbuffer::DX11Indexbuffer(void* indices, UInt32 count)
		:Indexbuffer(indices, count) {

		m_BindFunction = ST_BIND_EVENT_FN(Bind);
		m_UnbindFunction = ST_BIND_EVENT_FN(Unbind);
		m_CleanFunction = ST_BIND_EVENT_FN(Clean);

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

		DX11Common::GetDevice()->CreateBuffer(&description, &subresource, &m_IndexbufferPtr);
	}

	void DX11Indexbuffer::Bind() const {
		DX11Common::GetContext()->IASetIndexBuffer(m_IndexbufferPtr, DXGI_FORMAT_R32_UINT, 0);
	}

	void DX11Indexbuffer::Unbind() const {
		DX11Common::GetContext()->IASetIndexBuffer(nullptr, DXGI_FORMAT_R32_UINT, 0);
	}

	void DX11Indexbuffer::Clean() {
		m_IndexbufferPtr->Release();
	}
}
