#include "stpch.h"

#include "Sentinel/Graphics/Components/GraphicsContext.h"

#include "Platform/DirectX11/Graphics/Components/DX11GraphicsContext.h"

namespace Sentinel
{
	Scope<GraphicsContextBase> GraphicsContextUtils::Create(GLFWwindow* window) {
		switch (Backend::GetAPI())
		{
			case Backend::API::None:
				ST_ENGINE_ASSERT(false, "API::None currently not supported");
				return nullptr;
			case Backend::API::DirectX11:
				return CreateScope<DX11GraphicsContext>(window);
		}

		ST_ENGINE_ASSERT(false, "Unknown Backend API");
		return nullptr;
	}
}
