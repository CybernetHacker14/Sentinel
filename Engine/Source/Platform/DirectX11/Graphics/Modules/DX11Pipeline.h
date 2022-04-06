#pragma once

#include "Sentinel/Graphics/Modules/Pipeline.h"

#include "Platform/DirectX11/Graphics/Core/DX11Common.h"

namespace Sentinel
{
	class DX11Pipeline final : public Pipeline {
	public:
		DX11Pipeline();

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
