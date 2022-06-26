#include "stpch.h"
#include "Sentinel/Graphics/Common/GraphicsMemoryManager.h"

namespace Sentinel {
    GraphicsMemoryManager::GraphicsMemoryManager() {
        VertexbufferAllocator.AllocateMemoryBlock(10);
        IndexbufferAllocator.AllocateMemoryBlock(10);
        LayoutAllocator.AllocateMemoryBlock(10);
        ConstantbufferAllocator.AllocateMemoryBlock(10);

        ShaderAllocator.AllocateMemoryBlock(5);
        Texture2DAllocator.AllocateMemoryBlock(16);
    }

    GraphicsMemoryManager::~GraphicsMemoryManager() {
        VertexbufferAllocator.DeleteAll();
        VertexbufferAllocator.DeallocateMemoryBlock();
        IndexbufferAllocator.DeleteAll();
        IndexbufferAllocator.DeallocateMemoryBlock();
        LayoutAllocator.DeleteAll();
        LayoutAllocator.DeallocateMemoryBlock();
        ConstantbufferAllocator.DeleteAll();
        ConstantbufferAllocator.DeallocateMemoryBlock();

        ShaderAllocator.DeleteAll();
        ShaderAllocator.DeallocateMemoryBlock();
        Texture2DAllocator.DeleteAll();
        Texture2DAllocator.DeallocateMemoryBlock();
    }
}  // namespace Sentinel