#include "stpch.h"
#include "Sentinel/Graphics/Core/Backend.h"
#include "Sentinel/Graphics/Components/Buffers/Vertexbuffer.h"

#include "Platform/DirectX11/Graphics/Components/Buffers/DX11Vertexbuffer.h"

namespace Sentinel
{
	void Vertexbuffer::Bind(UInt32 stride) {
		if (Backend::GetAPI() == Backend::API::DirectX11)
		{
			BaseDowncast<DX11Vertexbuffer>()->Bind(stride);
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

	void Vertexbuffer::Unbind() {
		if (Backend::GetAPI() == Backend::API::DirectX11)
		{
			BaseDowncast<DX11Vertexbuffer>()->Unbind();
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

	void Vertexbuffer::SetData(const void* verticesData, UInt32 size) {
		if (Backend::GetAPI() == Backend::API::DirectX11)
		{
			BaseDowncast<DX11Vertexbuffer>()->SetData(verticesData, size);
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
