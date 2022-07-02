#include "stpch.h"
#include "Sentinel/Graphics/Common/GraphicsMemoryManager.h"

#include "Sentinel/Graphics/Device/ContextData.h"
#include "Sentinel/Graphics/Device/SwapchainData.h"

#include "Sentinel/Graphics/Buffer/VertexbufferData.h"
#include "Sentinel/Graphics/Buffer/VertexbufferLayoutData.h"
#include "Sentinel/Graphics/Buffer/IndexbufferData.h"
#include "Sentinel/Graphics/Buffer/ConstantbufferData.h"

#include "Sentinel/Graphics/Material/ShaderData.h"
#include "Sentinel/Graphics/Texture/Texture2DData.h"

namespace Sentinel {
    GraphicsMemoryManager::GraphicsMemoryManager() {
        ContextAllocator.AllocateMemoryBlock(10);
        SwapchainAllocator.AllocateMemoryBlock(10);

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

        SwapchainAllocator.DeleteAll();
        SwapchainAllocator.DeallocateMemoryBlock();
        ContextAllocator.DeleteAll();
        ContextAllocator.DeallocateMemoryBlock();
    }
}  // namespace Sentinel