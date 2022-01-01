#pragma once

#include "Sentinel/Graphics/Components/Materials/Texture2D.h"

#include "Platform/DirectX11/Graphics/Core/DX11Common.h"

namespace Sentinel
{
	class DX11Texture2D : public Texture2DCRTP<DX11Texture2D> {
	public:
		DX11Texture2D(const Texture2DImportSettings& settings);
		~DX11Texture2D();

		void Bind(UInt32 slot, const ShaderType shaderType) const;
		void Unbind(UInt32 slot, const ShaderType shaderType) const;

		Bool IsHDR() const { return m_HDR; }
		Bool IsLoaded() const { return m_Loaded; }

		UInt32 GetWidth() const { return m_Width; }
		UInt32 GetHeight() const { return m_Height; }
	private:
		void Load();

	private:
		friend class Texture2DCRTP<DX11Texture2D>;

		void* m_TexturePixels = nullptr;
		Bool m_Loaded = false;
		Bool m_HDR = false;
		UInt32 m_Width = 0, m_Height = 0;
		Texture2DImportSettings m_Settings;

		ID3D11ShaderResourceView* m_ResourceView;
	};
}
