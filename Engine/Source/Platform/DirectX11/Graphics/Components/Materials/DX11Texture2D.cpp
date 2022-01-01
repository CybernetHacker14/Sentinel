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

	void DX11Texture2D::Bind(UInt32 slot, const ShaderType shaderType) const {
		ID3D11DeviceContext* context = DX11Common::GetContext();
		switch (shaderType)
		{
			case ShaderType::VERTEX:    context->VSSetShaderResources(slot, 1, &m_ResourceView); break;
			case ShaderType::PIXEL:     context->PSSetShaderResources(slot, 1, &m_ResourceView); break;
			case ShaderType::COMPUTE:   context->CSSetShaderResources(slot, 1, &m_ResourceView); break;
			case ShaderType::NONE:      ST_ENGINE_ASSERT(false, "Invalid shader type"); break;
		}
	}

	void DX11Texture2D::Unbind(UInt32 slot, const ShaderType shaderType) const {
		ID3D11DeviceContext* context = DX11Common::GetContext();
		switch (shaderType)
		{
			case ShaderType::VERTEX:    context->VSSetShaderResources(slot, 1, nullptr); break;
			case ShaderType::PIXEL:     context->PSSetShaderResources(slot, 1, nullptr); break;
			case ShaderType::COMPUTE:   context->CSSetShaderResources(slot, 1, nullptr); break;
			case ShaderType::NONE:      ST_ENGINE_ASSERT(false, "Invalid shader type"); break;
		}
	}

	void DX11Texture2D::Load() {
		ID3D11Texture2D* texture2D = nullptr;

		D3D11_TEXTURE2D_DESC textureDescription;
		SecureZeroMemory(&textureDescription, sizeof(textureDescription));
		textureDescription.ArraySize = 1;
		textureDescription.SampleDesc.Count = 1;
		textureDescription.SampleDesc.Quality = 0;
		textureDescription.Usage = D3D11_USAGE_DEFAULT;
		textureDescription.CPUAccessFlags = 0;
		textureDescription.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		textureDescription.MiscFlags = m_Settings.GenerateMipMaps ? D3D11_RESOURCE_MISC_GENERATE_MIPS : 0;

		// Set Format
		int width, height, channels;
		const char* path = m_Settings.texturePath.c_str();
		m_HDR = stbi_is_hdr(path) ? true : false;
		m_TexturePixels = static_cast<void*>(stbi_load(path, &width, &height, &channels, 4));

		if (m_TexturePixels == nullptr)
		{
			ST_ENGINE_ERROR("Failed to load image at path : {0}", path);
			return;
		}

		textureDescription.Format = m_HDR ? DXGI_FORMAT_R32G32B32A32_FLOAT :
			(m_Settings.sRGB ? DXGI_FORMAT_R8G8B8A8_UNORM_SRGB : DXGI_FORMAT_R8G8B8A8_UNORM);

		m_Width = width;
		m_Height = height;
		stbi_set_flip_vertically_on_load(false);
		// \Set Format

		textureDescription.Width = m_Width;
		textureDescription.Height = m_Height;
		textureDescription.MipLevels = m_HDR ? 1 : 0;

		if (m_Settings.GenerateMipMaps)
			textureDescription.BindFlags |= D3D11_BIND_RENDER_TARGET;

		DX11Common::GetDevice()->CreateTexture2D(&textureDescription, nullptr, &texture2D);

		UInt32 rowPitch = m_Width * 4 * (m_HDR ? sizeof(float) : sizeof(unsigned char));
		DX11Common::GetContext()->UpdateSubresource(texture2D, 0, nullptr, m_TexturePixels, rowPitch, 0);

		D3D11_SHADER_RESOURCE_VIEW_DESC viewDescription;
		SecureZeroMemory(&viewDescription, sizeof(viewDescription));
		viewDescription.Format = textureDescription.Format;
		viewDescription.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		viewDescription.Texture2D.MostDetailedMip = 0;
		viewDescription.Texture2D.MipLevels = m_Settings.GenerateMipMaps ? -1 : 1;

		DX11Common::GetDevice()->CreateShaderResourceView(texture2D, &viewDescription, &m_ResourceView);

		if (m_Settings.GenerateMipMaps)
			DX11Common::GetContext()->GenerateMips(m_ResourceView);

		free(m_TexturePixels);
	}
}
