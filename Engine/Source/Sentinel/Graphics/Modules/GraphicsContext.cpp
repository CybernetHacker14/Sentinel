#include "stpch.h"

#include "Sentinel/Graphics/Core/Backend.h"
#include "Sentinel/Graphics/Modules/GraphicsContext.h"

#include "Platform/DirectX11/Graphics/Modules/DX11GraphicsContext.h"

namespace Sentinel
{
	void GraphicsContext::Init() {
		if (Backend::GetAPI() == Backend::API::DirectX11)
		{
			BaseDowncast<DX11GraphicsContext>()->Init();
		}
		else if (Backend::GetAPI() == Backend::API::None)
		{
			ST_ENGINE_ASSERT(false, "API::None currently not supported");
		}
		else
		{
			ST_ENGINE_ASSERT(false, "Unknown Backend API");
		}
	}

	const ContextInfo& GraphicsContext::GetContextInfo() {
		if (Backend::GetAPI() == Backend::API::DirectX11)
		{
			return BaseDowncast<DX11GraphicsContext>()->GetContextInfo();
		}
		else if (Backend::GetAPI() == Backend::API::None)
		{
			ST_ENGINE_ASSERT(false, "API::None currently not supported");
			return ContextInfo();
		}
		else
		{
			ST_ENGINE_ASSERT(false, "Unknown Backend API");
			return ContextInfo();
		}
	}

	Scope<GraphicsContext> GraphicsContext::Create(GLFWwindow* window) {
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
