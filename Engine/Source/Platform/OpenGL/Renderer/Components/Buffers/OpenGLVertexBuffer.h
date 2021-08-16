#pragma once

#include "Sentinel/Renderer/Components/Buffers/VertexBuffer.h"

namespace Sentinel
{
	class OpenGLVertexBuffer : public VertexBuffer {
	public:
		OpenGLVertexBuffer(uint32_t size);
		OpenGLVertexBuffer(void* vertices, uint32_t size);

		virtual ~OpenGLVertexBuffer();

		void Bind() const override;
		void Unbind() const override;

		void SetData(const void* data, uint32_t size);

	private:
		uint32_t m_VertexBufferID;
		BufferLayout m_Layout;
	};
}
