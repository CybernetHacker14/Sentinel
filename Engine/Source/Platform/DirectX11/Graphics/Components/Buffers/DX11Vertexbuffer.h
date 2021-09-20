#pragma once

#include "Sentinel/Base/Define.h"
#include "Sentinel/Graphics/Components/Buffers/Vertexbuffer.h"

#include "Platform/DirectX11/Graphics/Core/DX11Common.h"

namespace Sentinel
{
	class DX11Vertexbuffer final : public VertexbufferCRTP<DX11Vertexbuffer> {
	public:
		DX11Vertexbuffer(UInt size);
		DX11Vertexbuffer(void* verticesData, UInt size);

		~DX11Vertexbuffer();

		void Bind(UInt stride) const;
		void Unbind() const;
		void SetData(const void* vertices, UInt size);

	private:
		ID3D11Buffer* m_VertexbufferPtr;
	};
}
