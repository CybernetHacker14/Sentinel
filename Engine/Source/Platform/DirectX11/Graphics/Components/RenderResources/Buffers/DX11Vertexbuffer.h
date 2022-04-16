#pragma once

#include "Sentinel/Graphics/Components/RenderResources/Buffers/Vertexbuffer.h"

struct ID3D11Buffer;

namespace Sentinel
{
	class DX11Vertexbuffer final : public Vertexbuffer {
	public:
		DX11Vertexbuffer(UInt32 size);
		DX11Vertexbuffer(void* verticesData, UInt32 size);

	private:
		void Bind(UInt32 stride) const;
		void Unbind() const;
		void SetData(const void* vertices, UInt32 size);

		void Clean();
	private:
		ID3D11Buffer* m_VertexbufferPtr;
	};
}
