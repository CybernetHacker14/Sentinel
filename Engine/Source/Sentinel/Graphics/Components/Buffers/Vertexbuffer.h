#pragma once

#include "Sentinel/Base/Define.h"

namespace Sentinel
{
	template<typename T>
	class Vertexbuffer;

	enum class ShaderDataType {
		None = 0, Float, Float2, Float3, Float4, Mat3, Mat4, Int, Int2, Int3, Int4, Bool
	};

	static UInt ShaderDataTypeSize(ShaderDataType type) {
		switch (type)
		{
			case Sentinel::ShaderDataType::Float:	return 4;
			case Sentinel::ShaderDataType::Float2:	return 4 * 2;
			case Sentinel::ShaderDataType::Float3:	return 4 * 3;
			case Sentinel::ShaderDataType::Float4:	return 4 * 4;
			case Sentinel::ShaderDataType::Mat3:	return 4 * 3 * 3;
			case Sentinel::ShaderDataType::Mat4:	return 4 * 4 * 4;
			case Sentinel::ShaderDataType::Int:		return 4;
			case Sentinel::ShaderDataType::Int2:	return 4 * 2;
			case Sentinel::ShaderDataType::Int3:	return 4 * 3;
			case Sentinel::ShaderDataType::Int4:	return 4 * 4;
			case Sentinel::ShaderDataType::Bool:	return 1;
		}

		ST_ENGINE_ASSERT(false, "Unknown ShaderDataType!");
		return 0;
	}

	struct BufferElement {
		STL::string Name;
		ShaderDataType Type;
		UInt Size;
		ULLong Offset;
		Bool Normalized;

		BufferElement() = default;

		BufferElement(ShaderDataType type, const STL::string& name, Bool normalized = false)
			: Name(name), Type(type), Size(ShaderDataTypeSize(type)), Offset(0), Normalized(normalized) {}

		UInt GetComponentCount() const {
			switch (Type)
			{
				case ShaderDataType::Float:		return 1;
				case ShaderDataType::Float2:	return 2;
				case ShaderDataType::Float3:	return 3;
				case ShaderDataType::Float4:	return 4;
				case ShaderDataType::Mat3:		return 3;
				case ShaderDataType::Mat4:		return 4;
				case ShaderDataType::Int:		return 1;
				case ShaderDataType::Int2:		return 2;
				case ShaderDataType::Int3:		return 3;
				case ShaderDataType::Int4:		return 4;
				case ShaderDataType::Bool:		return 1;
			}

			ST_ENGINE_ASSERT(false, "Unknown ShaderDataType!");
			return 0;
		}
	};

	class BufferLayout {
	public:
		BufferLayout() {}

		BufferLayout(STL::initializer_list<BufferElement> elements)
			: m_Elements(elements) {}

		UInt GetStride() const { return m_Stride; }
		const STL::vector<BufferElement>& GetElements() const { return m_Elements; }

		STL::vector<BufferElement>::iterator begin() { return m_Elements.begin(); }
		STL::vector<BufferElement>::iterator end() { return m_Elements.end(); }
		STL::vector<BufferElement>::const_iterator begin() const { return m_Elements.begin(); }
		STL::vector<BufferElement>::const_iterator end() const { return m_Elements.end(); }

	private:
		void CalculateOffsetAndStride() {
			ULLong offset = 0;
			m_Stride = 0;

			for (auto& element : m_Elements)
			{
				element.Offset = offset;
				offset += element.Size;
				m_Stride += element.Size;
			}
		}

	private:
		STL::vector<BufferElement> m_Elements;
		UInt m_Stride = 0;
	};

	class VertexbufferBase : public IntrusiveRefObject {
	public:
		template<typename T>
		inline Vertexbuffer<T>* BaseDowncast() {
			static_assert(STL::is_base_of<Vertexbuffer<T>, T>::value
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

		inline void Unbind() const {
			underlying().Unbind();
		}

		inline void SetData(const void* verticesData, UInt size) {
			underlying().SetData(verticesData, size);
		}

		inline const BufferLayout& GetLayout() const {
			return underlying().GetLayout();
		}

		inline void SetLayout(const BufferLayout& layout) {
			underlying().SetLayout(layout);
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
		Ref<VertexbufferBase> Create(UInt size);
		Ref<VertexbufferBase> Create(void* vertices, UInt size);
	};
}
