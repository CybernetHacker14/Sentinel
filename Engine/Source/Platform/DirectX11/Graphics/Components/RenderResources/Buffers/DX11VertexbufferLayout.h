#pragma once

#include "Sentinel/Graphics/Components/RenderResources/Buffers/VertexbufferLayout.h"

struct ID3D11InputLayout;

namespace Sentinel
{
	class DX11VertexbufferLayout final : public VertexbufferLayout {
	public:
		DX11VertexbufferLayout();

	private:
		void CreateInputLayout(SharedRef<Shader> shader);
		void Bind() const;
		void Unbind() const;
		void Clean();

		inline UInt32 GetStride() const { return m_Stride; }

	private:
		ID3D11InputLayout* m_InputLayout;
	};
}
