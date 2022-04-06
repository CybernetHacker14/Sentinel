#include "stpch.h"
#include "Sentinel/Graphics/Core/Backend.h"
#include "Sentinel/Graphics/Components/Buffers/Indexbuffer.h"

#include "Platform/DirectX11/Graphics/Components/Buffers/DX11Indexbuffer.h"

namespace Sentinel
{
	Indexbuffer::Indexbuffer(void* indices, UInt32 count)
		: m_Count(count) {}

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
