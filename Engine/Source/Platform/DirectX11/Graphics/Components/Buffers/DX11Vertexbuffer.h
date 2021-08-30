#pragma once

#include "Sentinel/Base/Define.h"
#include "Sentinel/Graphics/Components/Buffers/Vertexbuffer.h"

#include "Platform/DirectX11/Graphics/Core/DX11Common.h"

namespace Sentinel
{
	class DX11Vertexbuffer final : public Vertexbuffer<DX11Vertexbuffer> {
	public:
		DX11Vertexbuffer(UInt size);
		DX11Vertexbuffer(void* verticesData, UInt size);

		~DX11Vertexbuffer();

		void Bind() const;
		void Unbind() const;
		void SetData(const void* vertices, UInt size);

		inline const BufferLayout& GetLayout() const { return m_Layout; }
		inline void SetLayout(const BufferLayout& layout) { m_Layout = layout; }

	private:
		ID3D11Buffer* m_VertexbufferPtr;
		BufferLayout m_Layout;
	};
}
