#pragma once

#include "Sentinel/Common/Common.h"

namespace Sentinel
{
	class Indexbuffer : public ISharedRef {
	public:
		inline void Bind() const {
			if (!m_BindFunction)
				return;

			m_BindFunction();
		}

		inline void Unbind() const {
			if (!m_UnbindFunction)
				return;

			m_UnbindFunction();
		}

		inline void Clean() const {
			if (!m_CleanFunction)
				return;

			m_CleanFunction();
		}

		inline const UInt32 GetCount() const { return m_Count; }

	public:
		static SharedRef<Indexbuffer> Create(void* indices, UInt32 count);

	protected:
		Indexbuffer(void* indices, UInt32 count);

	protected:
		STL::delegate<void()> m_BindFunction;
		STL::delegate<void()> m_UnbindFunction;
		STL::delegate<void()> m_CleanFunction;

	protected:
		UInt32 m_Count;
	};
}
