#pragma once

#include "Sentinel/Renderer/Components/Buffers/IndexBuffer.h"
#include "Platform/DirectX11/Renderer/Core/DirectX11Internal.h"

namespace Sentinel
{
	class DirectX11IndexBuffer : public IndexBuffer {
	public:
		DirectX11IndexBuffer(void* indices, UInt count);
		virtual ~DirectX11IndexBuffer();

		void Bind() const override;
		void Unbind() const override;

		UInt GetCount() const override { return m_Count; }

	private:
		ID3D11Buffer* m_IndexBufferID;
		UInt m_Count;
	};
}
