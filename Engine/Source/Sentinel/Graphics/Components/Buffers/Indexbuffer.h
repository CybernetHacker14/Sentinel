#pragma once

#include "Sentinel/Base/Define.h"

namespace Sentinel
{
	template<typename T>
	class Indexbuffer;

	class IndexbufferBase : public IntrusiveRefObject {
	public:
		template<typename T>
		inline Indexbuffer<T>* BaseDowncast() {
			static_assert(STL::is_base_of<Indexbuffer<T>, T>::value
				"Operation not allowed. 'T' should be a derived from Indexbuffer<T>.");
			return static_cast<Indexbuffer<T>*>(this);
		}

		template<typename T>
		inline T* DerivedDowncast() {
			static_assert(STL::is_base_of<Indexbuffer<T>, T>::value,
				"Operation not allowed. 'T' should be a derived from Indexbuffer<T>.");
			return static_cast<T*>(this);
		}

	protected:
		IndexbufferBase() = default;
	};

	template<typename T>
	class Indexbuffer : public IndexbufferBase {
	public:
		inline void Bind() const {
			underlying().Bind();
		}

		inline void Unbind() const {
			underlying().Unbind();
		}

		inline UInt GetCount() const {
			return underlying().GetCount();
		}

	private:
		friend T;
		Indexbuffer() = default;

		inline T& underlying() {
			return static_cast<T&>(*this);
		}
	};

	class IndexbufferUtils {
	public:
		Ref<IndexbufferBase> Create(void* indices, UInt count);
	};
}
