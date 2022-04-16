#include "stpch.h"
#include "Sentinel/Graphics/Definitions/Backend.h"
#include "Sentinel/Graphics/Components/RenderResources/Buffers/Constantbuffer.h"

#include "Platform/DirectX11/Graphics/Components/RenderResources/Buffers/DX11Constantbuffer.h"

namespace Sentinel
{
	Constantbuffer::Constantbuffer(UInt32 size, UInt32 bindSlot, UsageType usageType)
		:m_Data(nullptr), m_Size(size), m_UsageType(usageType) {}

	SharedRef<Constantbuffer> Constantbuffer::Create(UInt32 size, UInt32 bindSlot, UsageType usageType) {
		switch (Backend::GetAPI())
		{
			case Backend::API::DirectX11:
				return CreateSharedRef<DX11Constantbuffer>(size, bindSlot, usageType);
			case Backend::API::None:
				ST_ENGINE_ASSERT(false, "API::None currently not supported");
				return nullptr;
		}

		ST_ENGINE_ASSERT(false, "Unknown Backend API");
		return nullptr;
	}
}
