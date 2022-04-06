#pragma once

#include "Sentinel/Graphics/Components/Buffers/Indexbuffer.h"

#include "Platform/DirectX11/Graphics/Core/DX11Common.h"

namespace Sentinel
{
	class DX11Indexbuffer final : public Indexbuffer {
	public:
		DX11Indexbuffer(void* indices, UInt32 count);

	private:
		void Bind() const;
		void Unbind() const;
		void Clean();

		inline UInt32 GetCount() { return m_Count; }

	private:
		ID3D11Buffer* m_IndexbufferPtr;
	};
}
