#pragma once

#include "Sentinel/Graphics/Components/Buffers/Vertexbuffer.h"

#include "Platform/DirectX11/Graphics/Core/DX11Common.h"

namespace Sentinel
{
	class DX11Vertexbuffer final : public VertexbufferCRTP<DX11Vertexbuffer> {
	public:
		DX11Vertexbuffer(UInt32 size);
		DX11Vertexbuffer(void* verticesData, UInt32 size);

	private:
		void Bind(UInt32 stride) const;
		void Unbind() const;
		void SetData(const void* vertices, UInt32 size);

		void Clean();
	private:
		friend class VertexbufferCRTP<DX11Vertexbuffer>;

		ID3D11Buffer* m_VertexbufferPtr;
	};
}
