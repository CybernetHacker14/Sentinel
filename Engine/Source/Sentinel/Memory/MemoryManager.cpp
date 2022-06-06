#include "stpch.h"
#include "Sentinel/Memory/MemoryManager.h"

namespace Sentinel {
    MemoryManager::MemoryManager() { 
        VertexbufferAllocator.AllocateMemoryBlock(10);
        IndexbufferAllocator.AllocateMemoryBlock(10);
        LayoutAllocator.AllocateMemoryBlock(10);
        ConstantbufferAllocator.AllocateMemoryBlock(10);
        ShaderAllocator.AllocateMemoryBlock(5);
        Texture2DAllocator.AllocateMemoryBlock(16); }

    MemoryManager::~MemoryManager() {
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