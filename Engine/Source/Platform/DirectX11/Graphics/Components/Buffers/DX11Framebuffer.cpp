#include "stpch.h"
#include "Platform/DirectX11/Graphics/Components/Buffers/DX11Framebuffer.h"

namespace Sentinel
{
	static const UInt32 s_MaxFramebufferSize = 8192; // 8K

	namespace Utils
	{
		static void AttachColorTexture(TextureSpecification texSpec, FramebufferSpecification spec,
			Attachments::ColorAttachment* outColorAttachment) {

			D3D11_TEXTURE2D_DESC texDescription;
			SecureZeroMemory(&texDescription, sizeof(texDescription));
			texDescription.Width = spec.Width;
			texDescription.Height = spec.Height;
			texDescription.MipLevels = 1;
			texDescription.ArraySize = 1;
			texDescription.Format = static_cast<DXGI_FORMAT>(texSpec.TextureFormat);
			texDescription.SampleDesc.Count = 1;
			texDescription.SampleDesc.Quality = 0;
			texDescription.Usage = D3D11_USAGE_DEFAULT;
			texDescription.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
			texDescription.CPUAccessFlags = 0;
			texDescription.MiscFlags = 0;

			DX11Common::GetDevice()->CreateTexture2D(&texDescription, nullptr, &outColorAttachment->RenderTexture);

			// RTV
			D3D11_RENDER_TARGET_VIEW_DESC rTVDescription;
			SecureZeroMemory(&rTVDescription, sizeof(rTVDescription));
			rTVDescription.Format = static_cast<DXGI_FORMAT>(texSpec.TextureFormat);
			rTVDescription.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
			rTVDescription.Texture2D.MipSlice = 0;
			DX11Common::GetDevice()->CreateRenderTargetView(outColorAttachment->RenderTexture.Get(),
				&rTVDescription, &outColorAttachment->RenderTargetView);

			// SRV
			{
				D3D11_SHADER_RESOURCE_VIEW_DESC sRVDescription;
				SecureZeroMemory(&sRVDescription, sizeof(sRVDescription));
				sRVDescription.Format = static_cast<DXGI_FORMAT>(texSpec.TextureFormat);
				sRVDescription.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
				sRVDescription.Texture2D.MipLevels = 1;
				sRVDescription.Texture2D.MostDetailedMip = 0;
				DX11Common::GetDevice()->CreateShaderResourceView(outColorAttachment->RenderTexture.Get(),
					&sRVDescription, &outColorAttachment->ShaderResourceView);
			}
		}

		static void AttachDepthTexture(TextureSpecification texSpec, FramebufferSpecification spec,
			Attachments::DepthAttachment* outDepthAttachment) {

			D3D11_TEXTURE2D_DESC texDescription;
			SecureZeroMemory(&texDescription, sizeof(texDescription));
			texDescription.Width = spec.Width;
			texDescription.Height = spec.Height;
			texDescription.MipLevels = 1;
			texDescription.ArraySize = 1;
			texDescription.Format = static_cast<DXGI_FORMAT>(texSpec.TextureFormat);
			texDescription.SampleDesc.Count = 1;
			texDescription.SampleDesc.Quality = 0;
			texDescription.Usage = D3D11_USAGE_DEFAULT;
			texDescription.CPUAccessFlags = 0;
			texDescription.MiscFlags = 0;
			texDescription.BindFlags = D3D11_BIND_DEPTH_STENCIL;

			if (static_cast<DXGI_FORMAT>(texSpec.TextureFormat) == DXGI_FORMAT_R32_TYPELESS)
				texDescription.BindFlags |= D3D11_BIND_SHADER_RESOURCE;

			D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilDescription;
			SecureZeroMemory(&depthStencilDescription, sizeof(depthStencilDescription));
			depthStencilDescription.Format = static_cast<DXGI_FORMAT>(texSpec.TextureFormat);
			depthStencilDescription.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
			depthStencilDescription.Texture2D.MipSlice = 0;

			DX11Common::GetDevice()->CreateDepthStencilView(outDepthAttachment->DepthStencilBuffer.Get(),
				&depthStencilDescription, &outDepthAttachment->DepthStencilView);
		}

		static void AttachToSwapchain(Attachments::ColorAttachment* outColorAttachment, FramebufferSpecification spec) {
			ID3D11Texture2D* backbuffer = nullptr;
			DX11Common::GetSwapchain()->GetBuffer(0, __uuidof(ID3D11Resource), (LPVOID*)&backbuffer);
			DX11Common::GetDevice()->CreateRenderTargetView(backbuffer, nullptr, &outColorAttachment->RenderTargetView);
			backbuffer->Release();
			backbuffer = nullptr;
		}

		static Bool IsColorFormat(TextureFormat format) {
			return (format == TextureFormat::RGBA32F
				|| format == TextureFormat::RGBA8UNORM
				|| format == TextureFormat::R32SINT);
		}

		static Bool IsDepthFormat(TextureFormat format) {
			return (format == TextureFormat::D24S8UINT);
		}
	}

	DX11Framebuffer::DX11Framebuffer(const FramebufferSpecification& specification)
		: Framebuffer(specification) {

		m_InvalidateFunction = ST_BIND_EVENT_FN(Invalidate);
		m_BindFunction = ST_BIND_EVENT_FN(Bind);
		m_UnbindFunction = ST_BIND_EVENT_FN(Unbind);

		m_BindColorBufferFunction = ST_BIND_EVENT_FN(BindColorBuffer);
		m_BindDepthBufferFunction = ST_BIND_EVENT_FN(BindDepthBuffer);
		m_UnbindBufferFunction = ST_BIND_EVENT_FN(UnbindBuffer);

		m_ResizeFunction = ST_BIND_EVENT_FN(Resize);
		m_GetColorAttachmentFunction = ST_BIND_EVENT_FN(GetColorAttachment);
		m_ClearFunction = ST_BIND_EVENT_FN(Clear);

		SecureZeroMemory(&m_Viewport, sizeof(m_Viewport));

		for (const TextureSpecification& spec : specification.Attachments.Attachments)
		{
			if (!Utils::IsDepthFormat(spec.TextureFormat))
				m_ColorAttachmentSpecifications.emplace_back(spec);
			else
				m_DepthAttachmentSpecification = spec;
		}

		Invalidate();
	}

	void DX11Framebuffer::Invalidate() {
		Clean();
		m_Viewport.TopLeftX = 0.0f;
		m_Viewport.TopLeftY = 0.0f;
		m_Viewport.Width = static_cast<float>(m_Specification.Width);
		m_Viewport.Height = static_cast<float>(m_Specification.Height);
		m_Viewport.MinDepth = 0.0f;
		m_Viewport.MaxDepth = 1.0f;

		if (!m_ColorAttachmentSpecifications.empty())
		{
			m_ColorAttachments.resize(m_ColorAttachmentSpecifications.size());

			for (UInt32 i = 0; i < m_ColorAttachments.size(); i++)
			{
				if (m_Specification.SwapchainTarget)
				{
					Utils::AttachToSwapchain(&m_ColorAttachments[i], m_Specification);
					break;
				}

				if (Utils::IsColorFormat(m_ColorAttachmentSpecifications[i].TextureFormat))
					Utils::AttachColorTexture(m_ColorAttachmentSpecifications[i], m_Specification, &m_ColorAttachments[i]);
			}
		}

		if (m_DepthAttachmentSpecification.TextureFormat != TextureFormat::NONE
			&& Utils::IsDepthFormat(m_DepthAttachmentSpecification.TextureFormat))
		{
			Utils::AttachDepthTexture(m_DepthAttachmentSpecification, m_Specification, &m_DepthAttachment);
		}
	}

	void DX11Framebuffer::Bind() const {
		DX11Common::GetContext()->RSSetViewports(1, &m_Viewport);

		// Need to understand whats happening here, maybe one RTV is for the actual rendering,
		// theother RTV is the entity ID mask, though not sure why just 2.
		Microsoft::WRL::ComPtr<ID3D11RenderTargetView> pRenderViews[2];
		for (UInt32 i = 0; i < m_ColorAttachments.size(); i++)
			pRenderViews[i] = m_ColorAttachments[i].RenderTargetView;

		DX11Common::GetContext()->OMSetRenderTargets(static_cast<UInt32>(m_ColorAttachments.size()),
			pRenderViews[0].GetAddressOf(), m_DepthAttachment.DepthStencilView.Get());
	}

	void DX11Framebuffer::Unbind() const {
		// Expects an array of ID3D11RenderTargetView*, Direct nullptr clearly isn't
		ID3D11RenderTargetView* null = nullptr;
		DX11Common::GetContext()->OMSetRenderTargets(1, &null, NULL);
	}

	void DX11Framebuffer::BindColorBuffer(UInt32 index, UInt32 slot, ShaderType shaderType) {
		switch (shaderType)
		{
			case ShaderType::VERTEX:
				DX11Common::GetContext()->VSSetShaderResources(slot, 1, m_ColorAttachments[index].ShaderResourceView.GetAddressOf());
				break;

			case ShaderType::PIXEL:
				DX11Common::GetContext()->PSSetShaderResources(slot, 1, m_ColorAttachments[index].ShaderResourceView.GetAddressOf());
				break;

			case ShaderType::COMPUTE:
				DX11Common::GetContext()->CSSetShaderResources(slot, 1, m_ColorAttachments[index].ShaderResourceView.GetAddressOf());
				break;

			case ShaderType::NONE:
				ST_ENGINE_ASSERT(false, "Invalid shader type");
				break;
		}
	}

	void DX11Framebuffer::BindDepthBuffer(UInt32 slot, ShaderType shaderType) {
		switch (shaderType)
		{
			case ShaderType::VERTEX:
				DX11Common::GetContext()->VSSetShaderResources(slot, 1, m_DepthAttachment.ShaderResourceView.GetAddressOf());
				break;

			case ShaderType::PIXEL:
				DX11Common::GetContext()->PSSetShaderResources(slot, 1, m_DepthAttachment.ShaderResourceView.GetAddressOf());
				break;

			case ShaderType::COMPUTE:
				DX11Common::GetContext()->CSSetShaderResources(slot, 1, m_DepthAttachment.ShaderResourceView.GetAddressOf());
				break;

			case ShaderType::NONE:
				ST_ENGINE_ASSERT(false, "Invalid shader type");
				break;
		}
	}

	void DX11Framebuffer::UnbindBuffer(UInt32 slot, ShaderType shaderType) {
		switch (shaderType)
		{
			case ShaderType::VERTEX:
				DX11Common::GetContext()->VSSetShaderResources(slot, 1, nullptr);
				break;

			case ShaderType::PIXEL:
				DX11Common::GetContext()->PSSetShaderResources(slot, 1, nullptr);
				break;

			case ShaderType::COMPUTE:
				DX11Common::GetContext()->CSSetShaderResources(slot, 1, nullptr);
				break;

			case ShaderType::NONE:
				ST_ENGINE_ASSERT(false, "Invalid shader type");
				break;
		}
	}

	void DX11Framebuffer::Resize(UInt32 width, UInt32 height) {

		if (width == 0 || height == 0 || width > s_MaxFramebufferSize || height > s_MaxFramebufferSize)
			return;

		m_Specification.Width = width;
		m_Specification.Height = height;
		Invalidate();
	}

	void DX11Framebuffer::Clear() {
		for (UInt32 i = 0; i < m_ColorAttachments.size(); i++)
		{
			DX11Common::GetContext()->ClearRenderTargetView(
				m_ColorAttachments[i].RenderTargetView.Get(), (float*)&m_Specification.ClearColor);
		}

		if (m_DepthAttachmentSpecification.TextureFormat != TextureFormat::NONE)
		{
			DX11Common::GetContext()->ClearDepthStencilView(
				m_DepthAttachment.DepthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
		}
	}

	void DX11Framebuffer::Clean() {
		for (UInt32 i = 0; i < m_ColorAttachments.size(); i++)
		{
			m_ColorAttachments[i].RenderTexture.Reset();
			m_ColorAttachments[i].RenderTargetView.Reset();
			m_ColorAttachments[i].ShaderResourceView.Reset();
		}

		m_ColorAttachments.clear();

		m_DepthAttachment.DepthStencilView.Reset();
		m_DepthAttachment.DepthStencilBuffer.Reset();
	}
}
