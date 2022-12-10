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

    struct DepthTexture2DData;
    struct RenderTexture2DData;
    struct FramebufferData;
    struct ViewportData;

    class GraphicsMemoryManager final: public ISharedRef {
    public:
        GraphicsMemoryManager();
        ~GraphicsMemoryManager();

        const UInt32 GetTotalAllocations();
        const UInt32 GetTotalFreeCount();

        PoolAllocator<ContextData> ContextAllocator;
        PoolAllocator<SwapchainData> SwapchainAllocator;

        PoolAllocator<VertexbufferData> VertexbufferAllocator;
        PoolAllocator<VertexbufferLayoutData> LayoutAllocator;
        PoolAllocator<IndexbufferData> IndexbufferAllocator;
        PoolAllocator<ConstantbufferData> ConstantbufferAllocator;

        PoolAllocator<ShaderData> ShaderAllocator;
        PoolAllocator<Texture2DData> Texture2DAllocator;

        PoolAllocator<DepthTexture2DData> DepthTexture2DAllocator;
        PoolAllocator<RenderTexture2DData> RenderTexture2DAllocator;
        PoolAllocator<FramebufferData> FramebufferAllocator;
        PoolAllocator<ViewportData> ViewportAllocator;

    private:
        friend class ContextAPI;
        friend class SwapchainAPI;
        friend class VertexbufferAPI;
        friend class VertexbufferLayoutAPI;
        friend class IndexbufferAPI;
        friend class ConstantbufferAPI;
        friend class ShaderAPI;
        friend class Texture2DAPI;
        friend class RenderTexture2DAPI;
        friend class DepthTexture2DAPI;
        friend class FramebufferAPI;
        friend class ViewportAPI;
    };
}  // namespace Sentinel