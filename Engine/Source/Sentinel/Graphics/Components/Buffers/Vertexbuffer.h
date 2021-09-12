#pragma once

#include "Sentinel/Base/Define.h"

namespace Sentinel
{
	template<typename T>
	class Vertexbuffer;

	class VertexbufferBase : public IntrusiveRefObject {
	public:
		template<typename T>
		inline Vertexbuffer<T>* BaseDowncast() {
			static_assert(STL::is_base_of<Vertexbuffer<T>, T>::value,
				"Operation not allowed. 'T' should be a derived from Vertexbuffer<T>.");
			return static_cast<Vertexbuffer<T>*>(this);
		}

		template<typename T>
		inline T* DerivedDowncast() {
			static_assert(STL::is_base_of<Vertexbuffer<T>, T>::value,
				"Operation not allowed. 'T' should be a derived from Vertexbuffer<T>.");
			return static_cast<T*>(this);
		}

	protected:
		VertexbufferBase() = default;
	};

	template<typename T>
	class Vertexbuffer : public VertexbufferBase {
	public:
		inline void Bind() const {
			underlying().Bind();
		}

		inline void Unbind(UInt stride) const {
			underlying().Unbind(stride);
		}

		inline void SetData(const void* verticesData, UInt size) {
			underlying().SetData(verticesData, size);
		}

	private:
		friend T;
		Vertexbuffer() = default;

		inline T& underlying() {
			return static_cast<T&>(*this);
		}
	};

	class VertexbufferUtils {
	public:
		static Ref<VertexbufferBase> Create(UInt size);
		static Ref<VertexbufferBase> Create(void* vertices, UInt size);
	};
}
