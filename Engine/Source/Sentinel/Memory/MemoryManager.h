#pragma once

#include "Sentinel/Common/Common.h"
#include "Sentinel/Memory/PoolAllocator.h"

#include "Sentinel/Graphics/Definitions/RenderResources.h"
#include "Sentinel/Graphics/Definitions/FrameBackings.h"

#include "Sentinel/Graphics/Components/RenderResources/Materials/Texture2DData.h"

namespace Sentinel {
    class MemoryManager final: public ISharedRef {
    public:
        MemoryManager();
        ~MemoryManager();

    public:
        PoolAllocator<Texture2DData> Texture2DAllocator;
    };
}  // namespace Sentinel