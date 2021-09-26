#pragma once

#include "Sentinel/Base/Define.h"
#include "Sentinel/Graphics/Components/Buffers/Indexbuffer.h"

#include "Platform/DirectX11/Graphics/Core/DX11Common.h"

namespace Sentinel
{
	class DX11Indexbuffer final : public IndexbufferCRTP<DX11Indexbuffer> {
	public:
		DX11Indexbuffer(void* indices, UInt count);
		~DX11Indexbuffer();

	private:
		void Bind() const;
		void Unbind() const;

		inline UInt GetCount() { return m_Count; }

	private:
		friend class IndexbufferCRTP<DX11Indexbuffer>;

		ID3D11Buffer* m_IndexbufferPtr;
		UInt m_Count;
	};
}
