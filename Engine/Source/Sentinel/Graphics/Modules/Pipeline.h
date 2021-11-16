#pragma once

#include "Sentinel/Common/Common.h"
#include "Sentinel/Graphics/Components/Materials/Shader.h"

namespace Sentinel
{
	template<typename T>
	class PipelineCRTP;

	class Pipeline : public ISharedRef {
	public:
		void CreateInputLayout(SharedRef<Shader> shader);
		void Bind();
		void Unbind();
		UInt32 GetStride();

		static SharedRef<Pipeline> Create();

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
		inline void CreateInputLayout(SharedRef<Shader> shader) {
			underlying().CreateInputLayout(shader);
		}

		inline void Bind() {
			underlying().Bind();
		}

		inline void Unbind() {
			underlying().Unbind();
		}

		inline UInt32 GetStride() {
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
