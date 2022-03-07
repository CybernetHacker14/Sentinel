#pragma once

#include "Sentinel/Graphics/Components/Buffers/Constantbuffer.h"
#include "Platform/DirectX11/Graphics/Core/DX11Common.h"

namespace Sentinel
{
	class DX11Constantbuffer : public Constantbuffer {
	public:
		DX11Constantbuffer(UInt32 size, UInt32 bindSlot, UsageType usageType);

	private:
		void VSBind() const;
		void PSBind() const;
		void CSBind() const;
		void SetStaticData(void* data) const;
		void SetDynamicData(void* data) const;

	private:
		ID3D11Buffer* m_Buffer;
		UInt32 m_BindSlot;
	};
}
