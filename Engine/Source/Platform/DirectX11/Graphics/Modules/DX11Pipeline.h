#pragma once

#include "Sentinel/Base/Define.h"
#include "Sentinel/Graphics/Modules/Pipeline.h"

#include "Platform/DirectX11/Graphics/Core/DX11Common.h"

namespace Sentinel
{
	class DX11Pipeline : public Pipeline<DX11Pipeline> {
	public:
		DX11Pipeline();
		~DX11Pipeline();

		void CreateInputLayout(Ref<ShaderBase> shader);
		void Bind();
		void Unbind();

		inline UInt GetStride() { return m_Stride; }

	private:
		ID3D11InputLayout* m_InputLayout;
		UInt m_Stride;
	};
}
