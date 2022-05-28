#pragma once

#include "Sentinel/Common/Common.h"
#include "Sentinel/Graphics/Components/RenderResources/Materials/ShaderData.h"

#include <glm/glm.hpp>

namespace Sentinel
{
	enum class TextureFormat {
		NONE = 0,

		// Color
		RGBA32F = 2,
		RGBA8UNORM = 28,
		R32SINT = 43, // For detecting objects via click (RED_INTEGER)

		// Depth/Stencil
		D24S8UINT = 45,

		// Defaults
		Depth = D24S8UINT
	};

	struct TextureSpecification {
		TextureSpecification() = default;
		TextureSpecification(TextureFormat format)
			: TextureFormat(format) {}

		TextureFormat TextureFormat = TextureFormat::NONE;
	};

	struct AttachmentSpecification {
		AttachmentSpecification() = default;
		AttachmentSpecification(const STL::initializer_list<TextureSpecification> attachments)
			: Attachments(attachments) {}

		STL::vector<TextureSpecification> Attachments;
	};

	struct FramebufferSpecification {
		UInt32 Width = 0, Height = 0;
		AttachmentSpecification Attachments;
		UInt32 Samples = 1;
		Bool SwapchainTarget = false;
		glm::vec4 ClearColor = { 0.0f, 0.0f, 0.0f, 0.0f };
	};

	class Framebuffer : public ISharedRef {
	public:
		inline void Invalidate() const {
			if (!m_InvalidateFunction)
				return;

			m_InvalidateFunction();
		}

		inline void Bind() const {
			if (!m_BindFunction)
				return;

			m_BindFunction();
		}

		inline void Unbind() const {
			if (!m_UnbindFunction)
				return;

			m_UnbindFunction();
		}

		inline void BindColorBuffer(UInt32 index, UInt32 slot, ShaderType shaderType) const {
			if (!m_BindColorBufferFunction)
				return;

			m_BindColorBufferFunction(index, slot, shaderType);
		}

		inline void BindDepthBuffer(UInt32 slot, ShaderType shaderType) const {
			if (!m_BindDepthBufferFunction)
				return;

			m_BindDepthBufferFunction(slot, shaderType);
		}

		inline void UnbindBuffer(UInt32 slot, ShaderType shaderType) const {
			if (!m_UnbindBufferFunction)
				return;

			m_UnbindBufferFunction(slot, shaderType);
		}

		inline void Resize(UInt32 width, UInt32 height) const {
			if (!m_ResizeFunction)
				return;

			m_ResizeFunction(width, height);
		}

		inline void* GetColorAttachment(UInt32 index = 0) const {
			if (!m_GetColorAttachmentFunction)
				return nullptr;

			return m_GetColorAttachmentFunction(index);
		}

		inline void Clear() const {
			if (!m_ClearFunction)
				return;

			m_ClearFunction();
		}

		inline void Clean() const {
			if (!m_CleanFunction)
				return;

			m_CleanFunction();
		}

		inline const FramebufferSpecification& GetSpecification() const {
			return m_Specification;
		}

	public:
		static SharedRef<Framebuffer> Create(const FramebufferSpecification& specification);

	protected:
		Framebuffer(const FramebufferSpecification& specification);

	protected:
		STL::delegate<void()> m_InvalidateFunction;
		STL::delegate<void()> m_BindFunction;
		STL::delegate<void()> m_UnbindFunction;

		STL::delegate<void(UInt32, UInt32, ShaderType)> m_BindColorBufferFunction;
		STL::delegate<void(UInt32, ShaderType)> m_BindDepthBufferFunction;
		STL::delegate<void(UInt32, ShaderType)> m_UnbindBufferFunction;

		STL::delegate<void(UInt32, UInt32)> m_ResizeFunction;
		STL::delegate<void* (UInt32)> m_GetColorAttachmentFunction;
		STL::delegate<void* ()> m_GetDepthAttachmentFunction;
		STL::delegate<void()> m_ClearFunction;
		STL::delegate<void()> m_CleanFunction;

	protected:
		FramebufferSpecification m_Specification;
	};
}
