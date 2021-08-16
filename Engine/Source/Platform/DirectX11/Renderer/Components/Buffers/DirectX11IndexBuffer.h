#pragma once

#include "Sentinel/Renderer/Components/Buffers/IndexBuffer.h"
#include "Platform/DirectX11/Renderer/Core/DirectX11Internal.h"

namespace Sentinel
{
	class DirectX11IndexBuffer : public IndexBuffer {
	public:
		DirectX11IndexBuffer(void* indices, uint32_t count);
		virtual ~DirectX11IndexBuffer();

		void Bind() const override;
		void Unbind() const override;

		uint32_t GetCount() const override { return m_Count; }

	private:
		ID3D11Buffer* m_IndexBufferID;
		uint32_t m_Count;
	};
}
