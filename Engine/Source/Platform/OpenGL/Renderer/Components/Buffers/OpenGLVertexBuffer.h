#pragma once

#include "Sentinel/Renderer/Components/Buffers/VertexBuffer.h"

namespace Sentinel
{
	class OpenGLVertexBuffer : public VertexBuffer {
	public:
		OpenGLVertexBuffer(UInt size);
		OpenGLVertexBuffer(void* vertices, UInt size);

		virtual ~OpenGLVertexBuffer();

		void Bind() const override;
		void Unbind() const override;

		void SetData(const void* data, UInt size);

	private:
		UInt m_VertexBufferID;
		BufferLayout m_Layout;
	};
}
