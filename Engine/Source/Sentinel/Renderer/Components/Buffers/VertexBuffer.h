#pragma once

#include "Sentinel/Base/Define.h"

namespace Sentinel
{
	enum class ShaderDataType {
		None = 0, Float, Float2, Float3, Float4, Mat3, Mat4, Int, Int2, Int3, Int4, Bool
	};

	static uint32_t ShaderDataTypeSize(ShaderDataType type) {
		switch (type)
		{
			case ShaderDataType::Float: return 4;
			case ShaderDataType::Float2: return 4 * 2;
			case ShaderDataType::Float3: return 4 * 3;
			case ShaderDataType::Float4: return 4 * 4;
			case ShaderDataType::Mat3: return 4 * 3 * 3;
			case ShaderDataType::Mat4: return 4 * 4 * 4;
			case ShaderDataType::Int: return 4;
			case ShaderDataType::Int2: return 4 * 2;
			case ShaderDataType::Int3: return 4 * 3;
			case ShaderDataType::Int4: return 4 * 4;
			case ShaderDataType::Bool: return 1;
		}

		ST_ENGINE_ASSERT(false, "Unknown ShaderDataType!");
		return 0;
	}

	struct BufferElement {
		STL::string Name;
		ShaderDataType Type;
		UInt Size;
		ULLong Offset;
		bool Normalized;

		BufferElement() = default;

		BufferElement(ShaderDataType type, const STL::string& name, Bool normalized = false)
			:Name(name), Type(type), Size(ShaderDataTypeSize(type)), Offset(0), Normalized(normalized) {}

		UInt GetComponentCount() const {
			switch (Type)
			{
				case ShaderDataType::Float: return 1;
				case ShaderDataType::Float2: return 2;
				case ShaderDataType::Float3: return 3;
				case ShaderDataType::Float4: return 4;
				case ShaderDataType::Mat3: return 3;
				case ShaderDataType::Mat4: return 4;
				case ShaderDataType::Int: return 1;
				case ShaderDataType::Int2: return 2;
				case ShaderDataType::Int3: return 3;
				case ShaderDataType::Int4: return 4;
				case ShaderDataType::Bool: return 1;
			}

			ST_ENGINE_ASSERT(false, "Unknown ShaderDataType");
			return 0;
		}
	};

	class BufferLayout {
	public:
		BufferLayout() {}

		BufferLayout(STL::initializer_list<BufferElement> elements)
			:m_Elements(elements) {
			CalculateOffsetsAndStride();
		}

		uint32_t GetStride() const { return m_Stride; }
		const STL::vector<BufferElement>& GetElements() const { return m_Elements; }

		STL::vector<BufferElement>::iterator begin() { return m_Elements.begin(); }
		STL::vector<BufferElement>::iterator end() { return m_Elements.end(); }
		STL::vector<BufferElement>::const_iterator begin() const { return m_Elements.begin(); }
		STL::vector<BufferElement>::const_iterator end() const { return m_Elements.end(); }

	private:
		void CalculateOffsetsAndStride() {
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

	class VertexBuffer : public IntrusiveRefObject {
	public:
		virtual ~VertexBuffer() = default;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual void SetData(const void* data, UInt size) = 0;

		static Ref<VertexBuffer> Create(UInt size);
		static Ref<VertexBuffer> Create(void* vertices, UInt size);
	};
}
