#include "stpch.h"
#include "Sentinel/Graphics/Core/Backend.h"
#include "Sentinel/Graphics/Modules/RenderStageHandler.h"

#include "Platform/DirectX11/Graphics/Modules/DX11RenderStageHandler.h"

namespace Sentinel
{
	UniqueRef<RenderStageHandler> RenderStageHandler::Create() {
		switch (Backend::GetAPI())
		{
			case Backend::API::None:
				ST_ENGINE_ASSERT(false, "API::None currently not supported");
				return nullptr;
			case Backend::API::DirectX11:
				return CreateUniqueRef<DX11RenderStageHandler>();
		}

		ST_ENGINE_ASSERT(false, "Unknown Backend API");
		return nullptr;
	}
}
