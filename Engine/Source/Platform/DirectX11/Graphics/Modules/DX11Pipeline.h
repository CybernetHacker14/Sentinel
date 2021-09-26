#pragma once

#include "Sentinel/Base/Define.h"
#include "Sentinel/Graphics/Modules/Pipeline.h"

#include "Platform/DirectX11/Graphics/Core/DX11Common.h"

namespace Sentinel
{
	class DX11Pipeline final : public PipelineCRTP<DX11Pipeline> {
	public:
		DX11Pipeline();
		~DX11Pipeline();

	private:
		void CreateInputLayout(Ref<Shader> shader);
		void Bind();
		void Unbind();

		inline UInt GetStride() const { return m_Stride; }

	private:
		friend class PipelineCRTP<DX11Pipeline>;

		ID3D11InputLayout* m_InputLayout;
		UInt m_Stride;
	};
}
