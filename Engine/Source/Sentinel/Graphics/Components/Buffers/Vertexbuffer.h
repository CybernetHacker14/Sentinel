#pragma once

#include "Sentinel/Base/Define.h"

namespace Sentinel
{
	template<typename T>
	class VertexbufferCRTP;

	class Vertexbuffer : public IntrusiveRefObject {
	public:
		void Bind(UInt stride);
		void Unbind();
		void SetData(const void* verticesData, UInt size);

		static Ref<Vertexbuffer> Create(UInt size);
		static Ref<Vertexbuffer> Create(void* vertices, UInt size);

	protected:
		Vertexbuffer() = default;

	private:
		template<typename T>
		inline VertexbufferCRTP<T>* BaseDowncast() {
			static_assert(STL::is_base_of<VertexbufferCRTP<T>, T>::value,
				"Operation not allowed. 'T' should be derived from VertexbufferCRTP<T>.");
			return static_cast<VertexbufferCRTP<T>*>(this);
		}

		template<typename T>
		inline T* DerivedDowncast() {
			static_assert(STL::is_base_of<VertexbufferCRTP<T>, T>::value,
				"Operation not allowed. 'T' should be derived from VertexbufferCRTP<T>.");
			return static_cast<T*>(this);
		}
	};

	template<typename T>
	class VertexbufferCRTP : public Vertexbuffer {
	private:
		inline void Bind(UInt stride) {
			underlying().Bind(stride);
		}

		inline void Unbind() {
			underlying().Unbind();
		}

		inline void SetData(const void* verticesData, UInt size) {
			underlying().SetData(verticesData, size);
		}

	private:
		friend T;
		friend Vertexbuffer;
		VertexbufferCRTP() = default;

		inline T& underlying() {
			return static_cast<T&>(*this);
		}
	};
}
