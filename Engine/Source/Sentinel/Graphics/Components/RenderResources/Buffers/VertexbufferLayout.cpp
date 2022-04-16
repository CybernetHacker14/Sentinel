#include "stpch.h"
#include "Sentinel/Graphics/Definitions/Backend.h"
#include "Sentinel/Graphics/Components/RenderResources/Buffers/VertexbufferLayout.h"

#include "Platform/DirectX11/Graphics/Components/RenderResources/Buffers/DX11VertexbufferLayout.h"

namespace Sentinel
{
	SharedRef<VertexbufferLayout> VertexbufferLayout::Create() {
		switch (Backend::GetAPI())
		{
			case Backend::API::None:
				ST_ENGINE_ASSERT(false, "API::None currently not supported");
				return nullptr;
			case Backend::API::DirectX11:
				return CreateSharedRef<DX11VertexbufferLayout>();
		}

		ST_ENGINE_ASSERT(false, "Unknown Backend API");
		return nullptr;
	}
}
