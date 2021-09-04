#include "stpch.h"

#include "Sentinel/Graphics/Core/Backend.h"
#include "Sentinel/Graphics/Components/Buffers/Vertexbuffer.h"

#include "Platform/DirectX11/Graphics/Components/Buffers/DX11Vertexbuffer.h"

namespace Sentinel
{
	Ref<VertexbufferBase> VertexbufferUtils::Create(UInt size) {
		switch (Backend::GetAPI())
		{
			case Backend::API::None:
				ST_ENGINE_ASSERT(false, "API::None currently not supported");
				return nullptr;
			case Backend::API::DirectX11:
				return CreateRef<DX11Vertexbuffer>(size);
		}

		ST_ENGINE_ASSERT(false, "Unknown Backend API");
		return nullptr;
	}

	Ref<VertexbufferBase> VertexbufferUtils::Create(void* vertices, UInt size) {
		switch (Backend::GetAPI())
		{
			case Backend::API::None:
				ST_ENGINE_ASSERT(false, "API::None currently not supported");
				return nullptr;
			case Backend::API::DirectX11:
				return CreateRef<DX11Vertexbuffer>(vertices, size);
		}

		ST_ENGINE_ASSERT(false, "Unknown Backend API");
		return nullptr;
	}
}
