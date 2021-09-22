#pragma once

#include "Sentinel/Base/Define.h"

namespace Sentinel
{
	template<typename T>
	class IndexbufferCRTP;

	class Indexbuffer : public IntrusiveRefObject {
	public:
		void Bind();
		void Unbind();
		UInt GetCount();

		static Ref<Indexbuffer> Create(void* indices, UInt count);

	protected:
		Indexbuffer() = default;

	private:
		template<typename T>
		inline IndexbufferCRTP<T>* BaseDowncast() {
			static_assert(STL::is_base_of<IndexbufferCRTP<T>, T>::value,
				"Operation not allowed. 'T' should be derived from IndexbufferCRTP<T>.");
			return static_cast<IndexbufferCRTP<T>*>(this);
		}

		template<typename T>
		inline T* DerivedDowncast() {
			static_assert(STL::is_base_of<IndexbufferCRTP<T>, T>::value,
				"Operation not allowed. 'T' should be derived from IndexbufferCRTP<T>.");
			return static_cast<T*>(this);
		}
	};

	template<typename T>
	class IndexbufferCRTP : public Indexbuffer {
	private:
		inline void Bind() {
			underlying().Bind();
		}

		inline void Unbind() {
			underlying().Unbind();
		}

		inline UInt GetCount() {
			return underlying().GetCount();
		}

	private:
		friend T;
		friend Indexbuffer;
		IndexbufferCRTP() = default;

		inline T& underlying() {
			return static_cast<T&>(*this);
		}
	};
}
