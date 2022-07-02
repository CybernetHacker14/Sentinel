#pragma once

#include "Sentinel/Common/Common.h"
#include "Sentinel/Memory/PoolAllocator.h"

namespace Sentinel {
    struct ContextData;
    struct SwapchainData;
    struct VertexbufferData;
    struct VertexbufferLayoutData;
    struct IndexbufferData;
    struct ConstantbufferData;

    struct ShaderData;
    struct Texture2DData;

    class GraphicsMemoryManager final: public ISharedRef {
    public:
        GraphicsMemoryManager();
        ~GraphicsMemoryManager();

    private:
        PoolAllocator<ContextData> ContextAllocator;
        PoolAllocator<SwapchainData> SwapchainAllocator;

        PoolAllocator<VertexbufferData> VertexbufferAllocator;
        PoolAllocator<VertexbufferLayoutData> LayoutAllocator;
        PoolAllocator<IndexbufferData> IndexbufferAllocator;
        PoolAllocator<ConstantbufferData> ConstantbufferAllocator;

        PoolAllocator<ShaderData> ShaderAllocator;
        PoolAllocator<Texture2DData> Texture2DAllocator;

    private:
        friend class ContextAPI;
        friend class SwapchainAPI;
        friend class VertexbufferAPI;
        friend class VertexbufferLayoutAPI;
        friend class IndexbufferAPI;
        friend class ConstantbufferAPI;
        friend class ShaderAPI;
        friend class Texture2DAPI;
    };
}  // namespace Sentinel