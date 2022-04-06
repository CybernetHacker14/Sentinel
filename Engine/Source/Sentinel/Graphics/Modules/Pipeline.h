#pragma once

#include "Sentinel/Common/Common.h"
#include "Sentinel/Graphics/Components/Materials/Shader.h"

namespace Sentinel
{
	class Pipeline : public ISharedRef {
	public:
		inline void CreateInputLayout(SharedRef<Shader> shader) const {
			if (!m_CreateLayoutFunction)
				return;

			m_CreateLayoutFunction(shader);
		}

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

		const UInt32 GetStride() const { return m_Stride; }

	public:
		static SharedRef<Pipeline> Create();

	protected:
		STL::delegate<void(SharedRef<Shader>)> m_CreateLayoutFunction;
		STL::delegate<void()> m_BindFunction;
		STL::delegate<void()> m_UnbindFunction;
		STL::delegate<void()> m_CleanFunction;

	protected:
		UInt32 m_Stride;
	};
}
