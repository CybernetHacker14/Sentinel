#pragma once

#include "Sentinel/Base/Define.h"
#include "Sentinel/Graphics/Components/Structures/RenderData.h"

namespace Sentinel
{
	template<typename T>
	class Pipeline;

	class PipelineBase : public IntrusiveRefObject {
	public:
		template<typename T>
		inline Pipeline<T>* BaseDowncast() {
			static_assert(STL::is_base_of<Pipeline<T>, T>::value,
				"Operation not allowed. 'T' should be a derived from Pipeline<T>.");
			return static_cast<Pipeline<T>*>(this);
		}

		template<typename T>
		inline T* DerivedDowncast() {
			static_assert(STL::is_base_of<Pipeline<T>, T>::value,
				"Operation not allowed. 'T' should be a derived from Pipeline<T>.");
			return static_cast<T*>(this);
		}
	};

	template<typename T>
	class Pipeline : public PipelineBase {
	public:
		inline void CreateInputLayout(Ref<ShaderBase> shader) {
			underlying().CreateInputLayout(shader);
		}

		inline void Bind() {
			underlying().Bind();
		}

		inline void Unbind() {
			underlying().Unbind();
		}

		inline UInt GetStride() {
			return underlying().GetStride();
		}

	private:
		friend T;
		Pipeline() = default;

		inline T& underlying() {
			return static_cast<T&>(*this);
		}
	};

	class PipelineUtils {
	public:
		static Ref<PipelineBase> Create();
	};
}
