#pragma once

#include "Sentinel/Graphics/Components/FrameBackings/Buffers/Framebuffer.h"
#include "Platform/DirectX11/Graphics/Core/DX11Common.h"

#include <glm/glm.hpp>

namespace Sentinel
{
	namespace Attachments
	{
		struct ColorAttachment {
			Microsoft::WRL::ComPtr<ID3D11Texture2D> RenderTexture;
			Microsoft::WRL::ComPtr<ID3D11RenderTargetView> RenderTargetView;
			Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> ShaderResourceView;
			Microsoft::WRL::ComPtr<ID3D11UnorderedAccessView> UnorderedAccessView;
		};

		struct DepthAttachment {
			Microsoft::WRL::ComPtr<ID3D11Texture2D> DepthStencilBuffer;
			Microsoft::WRL::ComPtr<ID3D11DepthStencilView> DepthStencilView;
			Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> ShaderResourceView;
		};
	}

	class DX11Framebuffer final : public Framebuffer {
	public:
		DX11Framebuffer(const FramebufferSpecification& specification);

	private:
		void Invalidate();
		void Bind() const;
		void Unbind() const;

		void BindColorBuffer(UInt32 index, UInt32 slot, ShaderType shaderType);
		void BindDepthBuffer(UInt32 slot, ShaderType shaderType);
		void UnbindBuffer(UInt32 slot, ShaderType shaderType);

		void Resize(UInt32 width, UInt32 height);

		void Clear();
		void Clean();

		inline void* GetColorAttachment(UInt32 index = 0) const {
			return m_ColorAttachments[index].ShaderResourceView.Get();
		}

		inline void* GetDepthAttachment() const {
			return m_DepthAttachment.ShaderResourceView.Get();
		}

	private:
		STL::vector<TextureSpecification> m_ColorAttachmentSpecifications;
		STL::vector<Attachments::ColorAttachment> m_ColorAttachments;

		TextureSpecification m_DepthAttachmentSpecification;
		Attachments::DepthAttachment m_DepthAttachment;

		D3D11_VIEWPORT m_Viewport;
	};
}
