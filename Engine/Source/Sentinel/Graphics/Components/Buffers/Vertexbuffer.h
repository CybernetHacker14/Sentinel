#pragma once

#include "Sentinel/Common/Common.h"

namespace Sentinel
{
	template<typename T>
	class VertexbufferCRTP;

	class Vertexbuffer : public ISharedRef {
	public:
		void Bind(UInt32 stride);
		void Unbind();
		void SetData(const void* verticesData, UInt32 size);

		void Clean();

		static SharedRef<Vertexbuffer> Create(UInt32 size);
		static SharedRef<Vertexbuffer> Create(void* vertices, UInt32 size);

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
		inline void Bind(UInt32 stride) {
			underlying().Bind(stride);
		}

		inline void Unbind() {
			underlying().Unbind();
		}

		inline void SetData(const void* verticesData, UInt32 size) {
			underlying().SetData(verticesData, size);
		}

		inline void Clean() {
			underlying().Clean();
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
