#pragma once

#include "Sentinel/Renderer/Components/Buffers/IndexBuffer.h"

namespace Sentinel
{
	class OpenGLIndexBuffer : public IndexBuffer {
	public:
		OpenGLIndexBuffer(void* indices, UInt count);
		virtual ~OpenGLIndexBuffer();

		void Bind() const override;
		void Unbind() const override;

		UInt GetCount() const override { return m_Count; }

	private:
		UInt m_IndexBufferID;
		UInt m_Count;
	};
}
