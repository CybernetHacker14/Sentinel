#include "stpch.h"
#include "Sentinel/Graphics/Definitions/Backend.h"
#include "Sentinel/Graphics/Components/FrameBackings/Buffers/Framebuffer.h"

#include "Platform/DirectX11/Graphics/Components/FrameBackings/Buffers/DX11Framebuffer.h"

namespace Sentinel
{
	Framebuffer::Framebuffer(const FramebufferSpecification& specification)
		: m_Specification(specification) {}

	SharedRef<Framebuffer> Framebuffer::Create(const FramebufferSpecification& specification) {
		switch (Backend::GetAPI())
		{
			case Backend::API::None:
				ST_ENGINE_ASSERT(false, "API::None currently not supported");
				return nullptr;
			case Backend::API::DirectX11:
				return CreateSharedRef<DX11Framebuffer>(specification);
		}

		ST_ENGINE_ASSERT(false, "Unknown Backend API");
		return nullptr;
	}
}
