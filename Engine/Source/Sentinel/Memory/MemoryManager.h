#pragma once

#include "Sentinel/Common/Common.h"
#include "Sentinel/Memory/PoolAllocator.h"

#include "Sentinel/Graphics/Definitions/RenderResources.h"
#include "Sentinel/Graphics/Definitions/FrameBackings.h"

#include "Sentinel/Graphics/Components/RenderResources/Buffers/VertexbufferData.h"
#include "Sentinel/Graphics/Components/RenderResources/Buffers/VertexbufferLayoutData.h"
#include "Sentinel/Graphics/Components/RenderResources/Buffers/IndexbufferData.h"
#include "Sentinel/Graphics/Components/RenderResources/Buffers/ConstantbufferData.h"
#include "Sentinel/Graphics/Components/RenderResources/Materials/Texture2DData.h"
#include "Sentinel/Graphics/Components/RenderResources/Materials/ShaderData.h"

namespace Sentinel {
    class MemoryManager final: public ISharedRef {
    public:
        MemoryManager();
        ~MemoryManager();

    public:
        PoolAllocator<VertexbufferData> VertexbufferAllocator;
        PoolAllocator<VertexbufferLayoutData> LayoutAllocator;
        PoolAllocator<IndexbufferData> IndexbufferAllocator;
        PoolAllocator<ConstantbufferData> ConstantbufferAllocator;
        PoolAllocator<Texture2DData> Texture2DAllocator;
        PoolAllocator<ShaderData> ShaderAllocator;
    };
}  // namespace Sentinel