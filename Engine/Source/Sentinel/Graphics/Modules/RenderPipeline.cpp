#include "stpch.h"
#include "Sentinel/Graphics/Definitions/Backend.h"
#include "Sentinel/Graphics/Modules/RenderPipeline.h"

#include "Platform/DirectX11/Graphics/Modules/DX11RenderPipeline.h"

namespace Sentinel
{
	UniqueRef<RenderPipeline> RenderPipeline::Create() {
		switch (Backend::GetAPI())
		{
			case Backend::API::None:
				ST_ENGINE_ASSERT(false, "API::None currently not supported");
				return nullptr;
			case Backend::API::DirectX11:
				return CreateUniqueRef<DX11RenderPipeline>();
		}

		ST_ENGINE_ASSERT(false, "Unknown Backend API");
		return nullptr;
	}
}
