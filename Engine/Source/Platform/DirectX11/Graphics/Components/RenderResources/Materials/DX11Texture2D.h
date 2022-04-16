#pragma once

#include "Sentinel/Graphics/Components/RenderResources/Materials/Texture2D.h"

struct ID3D11SamplerState;
struct ID3D11ShaderResourceView;

namespace Sentinel
{
	class DX11Texture2D : public Texture2D {
	public:
		DX11Texture2D(const Texture2DImportSettings& settings);

	private:
		void Bind(UInt32 slot, const ShaderType shaderType);
		void Unbind(UInt32 slot, const ShaderType shaderType) const;

		void Load();
		void Clean();

	private:
		ID3D11SamplerState* m_SamplerState;
		ID3D11ShaderResourceView* m_ResourceView;
	};
}
