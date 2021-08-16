#pragma once

#include "Sentinel/Renderer/Components/Buffers/IndexBuffer.h"

namespace Sentinel
{
	class OpenGLIndexBuffer : public IndexBuffer {
	public:
		OpenGLIndexBuffer(void* indices, uint32_t count);
		virtual ~OpenGLIndexBuffer();

		void Bind() const override;
		void Unbind() const override;

		uint32_t GetCount() const override { return m_Count; }

	private:
		uint32_t m_IndexBufferID;
		uint32_t m_Count;
	};
}
