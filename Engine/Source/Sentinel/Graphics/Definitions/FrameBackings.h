#pragma once

#include "Sentinel/Common/Common.h"
#include "Sentinel/Window/Window.h"
#include "Sentinel/Graphics/Components/FrameBackings/Misc/GraphicsContext.h"
#include "Sentinel/Graphics/Components/FrameBackings/Buffers/Framebuffer.h"

namespace Sentinel
{
	struct FrameBackings final : public ISharedRef {
	public:
		inline static SharedRef<FrameBackings> Create() {
			return CreateSharedRef<FrameBackings>();
		}

	public:
		WindowProperties WindowProperties;
		FramebufferSpecification FramebufferSpecification;

		UniqueRef<Window> Window;
		UniqueRef<GraphicsContext> GraphicsContext;
		SharedRef<Framebuffer> Framebuffer;
	};
}
