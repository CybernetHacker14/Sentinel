#include "stpch.h"
#include "Platform/DirectX11/Graphics/Components/Materials/DX11Texture2D.h"

#include <stb_image.h>

namespace Sentinel
{
	DX11Texture2D::DX11Texture2D(const Texture2DImportSettings& settings)
		: m_Settings(settings) {
		Load();
	}

	DX11Texture2D::~DX11Texture2D() {
		if (m_ResourceView)
		{
			m_ResourceView->Release();
			m_ResourceView = nullptr;
		}
	}

	void DX11Texture2D::Bind(UInt32 slot, const ShaderType shaderType) const {}

	void DX11Texture2D::Unbind(UInt32 slot, const ShaderType shaderType) const {}

	DX11Texture2D::Load() {}

	DX11Texture2D::SetFormat(D3D11_TEXTURE2D_DESC& desc) {
		int width, height, channels;
		const char* path = m_Settings.texturePath.c_str();
		m_HDR = stbi_is_hdr(path) ? true : false;
		m_TexturePixels = static_cast<void*>(stbi_load(path, &width, &height, &channels, 4));

		if (m_TexturePixels == nullptr)
		{
			ST_ENGINE_ERROR("Failed to load image at path : {0}", path);
			return;
		}

		desc.Format = m_HDR ? DXGI_FORMAT_R32G32B32A32_FLOAT :
			(m_Settings.sRGB ? DXGI_FORMAT_R8G8B8A8_UNORM_SRGB : DXGI_FORMAT_R8G8B8A8_UNORM);

		m_Width = width;
		m_Height = height;
		stbi_set_flip_vertically_on_load(false);
	}
}
