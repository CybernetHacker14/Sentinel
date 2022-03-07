#include "stpch.h"
#include "Sentinel/Graphics/Core/Backend.h"
#include "Sentinel/Graphics/Components/Buffers/Indexbuffer.h"

#include "Platform/DirectX11/Graphics/Components/Buffers/DX11Indexbuffer.h"

namespace Sentinel
{
	void Indexbuffer::Bind() {
		if (Backend::GetAPI() == Backend::API::DirectX11)
		{
			BaseDowncast<DX11Indexbuffer>()->Bind();
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

	void Indexbuffer::Unbind() {
		if (Backend::GetAPI() == Backend::API::DirectX11)
		{
			BaseDowncast<DX11Indexbuffer>()->Unbind();
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

	void Indexbuffer::Clean() {
		if (Backend::GetAPI() == Backend::API::DirectX11)
		{
			BaseDowncast<DX11Indexbuffer>()->Clean();
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

	UInt32 Indexbuffer::GetCount() {
		if (Backend::GetAPI() == Backend::API::DirectX11)
		{
			return BaseDowncast<DX11Indexbuffer>()->GetCount();
		}
		else if (Backend::GetAPI() == Backend::API::None)
		{
			ST_ENGINE_ASSERT(false, "API::None currently not supported");
			return UInt32();
		}
		else
		{
			ST_ENGINE_ASSERT(false, "Unknown Backend API");
			return UInt32();
		}
	}

	SharedRef<Indexbuffer> Indexbuffer::Create(void* indices, UInt32 count) {
		switch (Backend::GetAPI())
		{
			case Backend::API::None:
				ST_ENGINE_ASSERT(false, "API::None currently not supported");
				return nullptr;
			case Backend::API::DirectX11:
				return CreateSharedRef<DX11Indexbuffer>(indices, count);
		}

		ST_ENGINE_ASSERT(false, "Unknown Backend API");
		return nullptr;
	}
}
