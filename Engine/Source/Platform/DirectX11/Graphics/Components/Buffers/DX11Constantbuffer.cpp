#include "stpch.h"
#include "Platform/DirectX11/Graphics/Components/Buffers/DX11Constantbuffer.h"

namespace Sentinel
{
	DX11Constantbuffer::DX11Constantbuffer(UInt32 size, UInt32 bindSlot, UsageType usageType)
		:m_BindSlot(bindSlot), Constantbuffer(size, bindSlot, usageType) {

		m_VSBindFunction = ST_BIND_EVENT_FN(VSBind);
		m_PSBindFunction = ST_BIND_EVENT_FN(PSBind);
		m_CSBindFunction = ST_BIND_EVENT_FN(CSBind);
		m_SetStaticDataFunction = ST_BIND_EVENT_FN(SetStaticData);
		m_SetDynamicDataFunction = ST_BIND_EVENT_FN(SetDynamicData);

		D3D11_BUFFER_DESC description;
		SecureZeroMemory(&description, sizeof(description));
		description.ByteWidth = m_Size;
		description.Usage = m_UsageType == UsageType::DEFAULT ? D3D11_USAGE_DEFAULT : D3D11_USAGE_DYNAMIC;
		description.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		description.CPUAccessFlags = m_UsageType == UsageType::DYNAMIC ? D3D11_CPU_ACCESS_WRITE : 0;
		description.MiscFlags = 0;
		description.StructureByteStride = 0;

		DX11Common::GetDevice()->CreateBuffer(&description, nullptr, &m_Buffer);
	}

	void DX11Constantbuffer::VSBind() const {
		DX11Common::GetContext()->VSSetConstantBuffers(m_BindSlot, 1, &m_Buffer);
	}

	void DX11Constantbuffer::PSBind() const {
		DX11Common::GetContext()->PSSetConstantBuffers(m_BindSlot, 1, &m_Buffer);
	}

	void DX11Constantbuffer::CSBind() const {
		DX11Common::GetContext()->CSSetConstantBuffers(m_BindSlot, 1, &m_Buffer);
	}

	void DX11Constantbuffer::SetStaticData(void* data) const {
		DX11Common::GetContext()->UpdateSubresource(m_Buffer, 0, 0, data, 0, 0);
	}

	void DX11Constantbuffer::SetDynamicData(void* data) const {
		D3D11_MAPPED_SUBRESOURCE subresource;
		SecureZeroMemory(&subresource, sizeof(subresource));
		DX11Common::GetContext()->Map(m_Buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &subresource);
		memcpy(subresource.pData, data, m_Size);
		DX11Common::GetContext()->Unmap(m_Buffer, 0);
	}
}
