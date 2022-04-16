#include "stpch.h"
#include "Sentinel/Graphics/Definitions/Backend.h"
#include "Sentinel/Graphics/Components/RenderResources/Buffers/Vertexbuffer.h"

#include "Platform/DirectX11/Graphics/Components/RenderResources/Buffers/DX11Vertexbuffer.h"

namespace Sentinel
{
	SharedRef<Vertexbuffer> Vertexbuffer::Create(UInt32 size) {
		switch (Backend::GetAPI())
		{
			case Backend::API::None:
				ST_ENGINE_ASSERT(false, "API::None currently not supported");
				return nullptr;
			case Backend::API::DirectX11:
				return CreateSharedRef<DX11Vertexbuffer>(size);
		}

		ST_ENGINE_ASSERT(false, "Unknown Backend API");
		return nullptr;
	}

	SharedRef<Vertexbuffer> Vertexbuffer::Create(void* vertices, UInt32 size) {
		switch (Backend::GetAPI())
		{
			case Backend::API::None:
				ST_ENGINE_ASSERT(false, "API::None currently not supported");
				return nullptr;
			case Backend::API::DirectX11:
				return CreateSharedRef<DX11Vertexbuffer>(vertices, size);
		}

		ST_ENGINE_ASSERT(false, "Unknown Backend API");
		return nullptr;
	}
}
