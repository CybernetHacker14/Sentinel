#pragma once

#include "Sentinel/Renderer/Components/Buffers/VertexBuffer.h"
#include "Platform/DirectX11/Renderer/Core/DirectX11Internal.h"

namespace Sentinel
{
	class DirectX11VertexBuffer : public VertexBuffer {
	public:
		DirectX11VertexBuffer(UInt size);
		DirectX11VertexBuffer(void* vertices, UInt size);

		virtual ~DirectX11VertexBuffer();

		void Bind() const override;
		void Unbind() const override;

		void SetData(const void* data, UInt size);
	private:
		ID3D11Buffer* m_VertexBufferID;
		BufferLayout m_Layout;
	};
}
