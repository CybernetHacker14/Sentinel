#pragma once

#include "Sentinel/Common/Common.h"

namespace Sentinel
{
	class Vertexbuffer : public ISharedRef {
	public:
		inline ~Vertexbuffer() {
			Clean();
		}

		inline void Bind(UInt32 stride) const {
			if (!m_BindFunction)
				return;

			m_BindFunction(stride);
		}

		inline void Unbind() const {
			if (!m_UnbindFunction)
				return;

			m_UnbindFunction();
		}

		inline void SetData(const void* verticesData, UInt32 size) {
			if (!m_SetDataFunction)
				return;

			m_SetDataFunction(verticesData, size);
		}

		inline void Clean() {
			if (!m_CleanFunction)
				return;

			m_CleanFunction();
		}

	public:
		static SharedRef<Vertexbuffer> Create(UInt32 size);
		static SharedRef<Vertexbuffer> Create(void* vertices, UInt32 size);

	protected:
		STL::delegate<void(UInt32)> m_BindFunction;
		STL::delegate<void()> m_UnbindFunction;
		STL::delegate<void(const void*, UInt32)> m_SetDataFunction;
		STL::delegate<void()> m_CleanFunction;
	};
}
