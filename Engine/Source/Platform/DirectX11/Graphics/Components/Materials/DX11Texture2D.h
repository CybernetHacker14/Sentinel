#pragma once

#include "Sentinel/Graphics/Components/Materials/Texture2D.h"

#include "Platform/DirectX11/Graphics/Core/DX11Common.h"

namespace Sentinel
{
	class DX11Texture2D : public Texture2D {
	public:
		DX11Texture2D(const Texture2DImportSettings& settings);

	private:
		void Bind(UInt32 slot, const ShaderType shaderType) const;
		void Unbind(UInt32 slot, const ShaderType shaderType) const;
		void Destructor();

		void Load();

	private:
		ID3D11ShaderResourceView* m_ResourceView;
	};
}
