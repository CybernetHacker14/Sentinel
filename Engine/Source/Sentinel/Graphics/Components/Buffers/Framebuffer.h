#pragma once

#include "Sentinel/Common/Common.h"
#include "Sentinel/Graphics/Components/Materials/Shader.h"

#include <glm/glm.hpp>

namespace Sentinel
{
	template<typename T>
	class FramebufferCRTP;

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
		void Invalidate();
		void Bind();
		void Unbind();

		void BindColorBuffer(UInt32 index, UInt32 slot, ShaderType shaderType);
		void BindDepthBuffer(UInt32 slot, ShaderType shaderType);
		void UnbindBuffer(UInt32 slot, ShaderType shaderType);

		void Resize(UInt32 width, UInt32 height);
		void* GetColorAttachment(UInt32 index = 0);
		void Clear();

		const FramebufferSpecification& GetSpecification();

		static SharedRef<Framebuffer> Create(const FramebufferSpecification& specification);
	public:
		template<typename T>
		inline T* DerivedDowncast() {
			static_assert(STL::is_base_of<FramebufferCRTP<T>, T>::value,
				"Operation not allowed. 'T' should be derived from FramebufferCRTP<T>.");
			return static_cast<T*>(this);
		}

	private:
		template<typename T>
		inline FramebufferCRTP<T>* BaseDowncast() {
			static_assert(STL::is_base_of<FramebufferCRTP<T>, T>::value,
				"Operation not allowed. 'T' should be derived from FramebufferCRTP<T>.");
			return static_cast<FramebufferCRTP<T>*>(this);
		}
	};

	template<typename T>
	class FramebufferCRTP : public Framebuffer {
	private:
		inline void Invalidate() {
			underlying().Invalidate();
		}

		inline void Bind() {
			underlying().Bind();
		}

		inline void Unbind() {
			underlying().Unbind();
		}

		inline void BindColorBuffer(UInt32 index, UInt32 slot, ShaderType shaderType) {
			underlying().BindColorBuffer(index, slot, shaderType);
		}

		inline void BindDepthBuffer(UInt32 slot, ShaderType shaderType) {
			underlying().BindDepthBuffer(slot, shaderType);
		}

		inline void UnbindBuffer(UInt32 slot, ShaderType shaderType) {
			underlying().UnbindBuffer(slot, shaderType);
		}

		inline void Resize(UInt32 width, UInt32 height) {
			underlying().Resize(width, height);
		}

		inline void* GetColorAttachment(UInt32 index = 0) {
			return underlying().GetColorAttachment(index);
		}

		inline void Clear() {
			underlying().Clear();
		}

		inline const FramebufferSpecification& GetSpecification() {
			return underlying().GetSpecification();
		}
	private:
		friend T;
		friend Framebuffer;
		FramebufferCRTP() = default;

		inline T& underlying() {
			return static_cast<T&>(*this);
		}
	};
}
