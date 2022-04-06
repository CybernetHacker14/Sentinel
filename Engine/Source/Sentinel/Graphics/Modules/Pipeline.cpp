#include "stpch.h"
#include "Sentinel/Graphics/Core/Backend.h"
#include "Sentinel/Graphics/Modules/Pipeline.h"

#include "Platform/DirectX11/Graphics/Modules/DX11Pipeline.h"

namespace Sentinel
{
	SharedRef<Pipeline> Pipeline::Create() {
		switch (Backend::GetAPI())
		{
			case Backend::API::None:
				ST_ENGINE_ASSERT(false, "API::None currently not supported");
				return nullptr;
			case Backend::API::DirectX11:
				return CreateSharedRef<DX11Pipeline>();
		}

		ST_ENGINE_ASSERT(false, "Unknown Backend API");
		return nullptr;
	}
}
