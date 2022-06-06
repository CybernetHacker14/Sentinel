#pragma once

#include "Sentinel/Common/Common.h"

namespace Sentinel
{
	struct RenderResources final : public ISharedRef {
	public:
		inline static SharedRef<RenderResources> Create() {
			return CreateSharedRef<RenderResources>();
		}
	};
}
