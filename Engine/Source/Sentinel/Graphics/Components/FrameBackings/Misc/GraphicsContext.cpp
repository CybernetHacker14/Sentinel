#include "stpch.h"
#include "Sentinel/Graphics/Definitions/Backend.h"
#include "Sentinel/Graphics/Components/FrameBackings/Misc/GraphicsContext.h"

#include "Platform/DirectX11/Graphics/Components/FrameBackings/Misc/DX11GraphicsContext.h"

namespace Sentinel
{
	UniqueRef<GraphicsContext> GraphicsContext::Create(GLFWwindow* window) {
		switch (Backend::GetAPI())
		{
			case Backend::API::None:
				ST_ENGINE_ASSERT(false, "API::None currently not supported");
				return nullptr;
			case Backend::API::DirectX11:
				return CreateUniqueRef<DX11GraphicsContext>(window);
		}

		ST_ENGINE_ASSERT(false, "Unknown Backend API");
		return nullptr;
	}
}
