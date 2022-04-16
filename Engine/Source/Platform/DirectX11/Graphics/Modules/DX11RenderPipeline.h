#pragma once

#include "Sentinel/Graphics/Modules/RenderPipeline.h"

namespace Sentinel
{
	class DX11RenderPipeline final : public RenderPipeline {
	public:
		DX11RenderPipeline();

	private:
		void CreateFrameBackings(const WindowProperties& windowProps,
			const FramebufferSpecification& framebufferSpecs);
		void CreateFrameBackingsComponents();
		void InitFrameBackings();

		void BindRenderResources();
		void BindFramebuffer();

		void Resize(UInt32 width, UInt32 height);
		void Draw();
		void SwapBuffers(Bool vSync = true);
		void Clear();

		void UnbindFramebuffer();
		void UnbindRenderResources();

		void CleanRenderResources();
		void CleanFrameBackings();
		void CleanNativeResources();
	};
}
