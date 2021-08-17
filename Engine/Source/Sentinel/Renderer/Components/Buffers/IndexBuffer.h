#pragma once

#include "Sentinel/Base/Define.h"

namespace Sentinel
{
	class IndexBuffer : public IntrusiveRefObject {
	public:
		virtual ~IndexBuffer() = default;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual UInt GetCount() const = 0;

		static Ref<IndexBuffer> Create(void* indices, UInt count);
	};
}
