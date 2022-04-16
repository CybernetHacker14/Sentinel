#pragma once

#include "Sentinel/Graphics/Components/RenderResources/Buffers/Constantbuffer.h"

struct ID3D11Buffer;

namespace Sentinel
{
	class DX11Constantbuffer final : public Constantbuffer {
	public:
		DX11Constantbuffer(UInt32 size, UInt32 bindSlot, UsageType usageType);

	private:
		void VSBind() const;
		void PSBind() const;
		void CSBind() const;
		void SetStaticData(void* data) const;
		void SetDynamicData(void* data) const;

		void Clean() const;

	private:
		ID3D11Buffer* m_Buffer;
		UInt32 m_BindSlot;
	};
}
