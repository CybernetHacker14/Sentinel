#pragma once

#include "Sentinel/Common/Common.h"
#include "Sentinel/Graphics/Modules/RenderPipeline.h"

namespace Sentinel
{
	class Renderer {
	public:
		Renderer();
		~Renderer();

		void SetRenderSpecifications(
			const WindowProperties& windowProps,
			const FramebufferSpecification& framebufferSpecs);

		void SubmitGeometryData(SharedRef<RenderResources> renderResources);

		void InitStartup();
		void PreRender();
		void FramebufferBind();
		void DrawCommand();
		void ClearCommand();
		void FramebufferUnbind();
		void PostRender();
		void InitShutdown();
		void Shutdown();

		void Resize(UInt32 width, UInt32 height);

	public:
		inline static UniqueRef<Renderer> Create() {
			return CreateUniqueRef<Renderer>();
		}

	public:
		inline Window& GetWindow() {
			return *(m_RenderPipeline->m_FrameBackings->Window);
		}

		inline const WindowProperties& GetWindowProperties() const {
			return m_RenderPipeline->m_FrameBackings->WindowProperties;
		}

		inline const FramebufferSpecification& GetFramebufferSpecification() const {
			return m_RenderPipeline->m_FrameBackings->Framebuffer->GetSpecification();
		}

	private:
		UniqueRef<RenderPipeline> m_RenderPipeline;
	};
}
