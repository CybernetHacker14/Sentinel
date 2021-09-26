#pragma once

#include "Sentinel/Base/Define.h"
#include "Sentinel/Graphics/Components/Materials/Shader.h"

namespace Sentinel
{
	template<typename T>
	class PipelineCRTP;

	class Pipeline : public IntrusiveRefObject {
	public:
		void CreateInputLayout(Ref<Shader> shader);
		void Bind();
		void Unbind();
		UInt GetStride();

		static Ref<Pipeline> Create();

	protected:
		Pipeline() = default;

	private:
		template<typename T>
		inline PipelineCRTP<T>* BaseDowncast() {
			static_assert(STL::is_base_of<PipelineCRTP<T>, T>::value,
				"Operation not allowed. 'T' should be a derived from PipelineCRTP<T>.");
			return static_cast<PipelineCRTP<T>*>(this);
		}

		template<typename T>
		inline T* DerivedDowncast() {
			static_assert(STL::is_base_of<PipelineCRTP<T>, T>::value,
				"Operation not allowed. 'T' should be a derived from PipelineCRTP<T>.");
			return static_cast<T*>(this);
		}
	};

	template<typename T>
	class PipelineCRTP : public Pipeline {
	private:
		inline void CreateInputLayout(Ref<Shader> shader) {
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
		friend Pipeline;
		PipelineCRTP() = default;

		inline T& underlying() {
			return static_cast<T&>(*this);
		}
	};
}
