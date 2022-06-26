#pragma once

#include "Sentinel/Common/Common.h"
#include "Sentinel/Memory/PoolAllocator.h"

#include "Sentinel/Graphics/Buffer/VertexbufferData.h"
#include "Sentinel/Graphics/Buffer/VertexbufferLayoutData.h"
#include "Sentinel/Graphics/Buffer/IndexbufferData.h"
#include "Sentinel/Graphics/Buffer/ConstantbufferData.h"

#include "Sentinel/Graphics/Material/ShaderData.h"
#include "Sentinel/Graphics/Texture/Texture2DData.h"

namespace Sentinel {
    template<typename T>
    class PoolAllocator;

    class GraphicsMemoryManager final: public ISharedRef {
    public:
        GraphicsMemoryManager();
        ~GraphicsMemoryManager();

    private:
        PoolAllocator<VertexbufferData> VertexbufferAllocator;
        PoolAllocator<VertexbufferLayoutData> LayoutAllocator;
        PoolAllocator<IndexbufferData> IndexbufferAllocator;
        PoolAllocator<ConstantbufferData> ConstantbufferAllocator;

        PoolAllocator<ShaderData> ShaderAllocator;
        PoolAllocator<Texture2DData> Texture2DAllocator;

    private:
        friend class VertexbufferAPI;
        friend class VertexbufferLayoutAPI;
        friend class IndexbufferAPI;
        friend class ConstantbufferAPI;
        friend class ShaderAPI;
        friend class Texture2DAPI;
    };
}  // namespace Sentinel