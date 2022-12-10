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

#include "Sentinel/Graphics/Texture/RenderTexture2DData.h"
#include "Sentinel/Graphics/Texture/DepthTexture2DData.h"
#include "Sentinel/Graphics/Output/FramebufferData.h"
#include "Sentinel/Graphics/Output/ViewportData.h"

namespace Sentinel {
    GraphicsMemoryManager::GraphicsMemoryManager() {
        ContextAllocator.AllocateMemoryBlock(2);
        SwapchainAllocator.AllocateMemoryBlock(2);

        VertexbufferAllocator.AllocateMemoryBlock(3);
        IndexbufferAllocator.AllocateMemoryBlock(2);
        LayoutAllocator.AllocateMemoryBlock(2);
        ConstantbufferAllocator.AllocateMemoryBlock(2);

        ShaderAllocator.AllocateMemoryBlock(2);
        Texture2DAllocator.AllocateMemoryBlock(3);

        RenderTexture2DAllocator.AllocateMemoryBlock(1);
        DepthTexture2DAllocator.AllocateMemoryBlock(1);

        ViewportAllocator.AllocateMemoryBlock(2);
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

        RenderTexture2DAllocator.DeleteAll();
        RenderTexture2DAllocator.DeallocateMemoryBlock();

        DepthTexture2DAllocator.DeleteAll();
        DepthTexture2DAllocator.DeallocateMemoryBlock();

        ViewportAllocator.DeleteAll();
        ViewportAllocator.DeallocateMemoryBlock();

        SwapchainAllocator.DeleteAll();
        SwapchainAllocator.DeallocateMemoryBlock();
        ContextAllocator.DeleteAll();
        ContextAllocator.DeallocateMemoryBlock();
    }

    const UInt32 GraphicsMemoryManager::GetTotalAllocations() {
        return VertexbufferAllocator.GetTotalAllocations() + IndexbufferAllocator.GetTotalAllocations() +
               LayoutAllocator.GetTotalAllocations() + ConstantbufferAllocator.GetTotalAllocations() +
               ShaderAllocator.GetTotalAllocations() + Texture2DAllocator.GetTotalAllocations() +
               RenderTexture2DAllocator.GetTotalAllocations() + DepthTexture2DAllocator.GetTotalAllocations() +
               ContextAllocator.GetTotalAllocations() + SwapchainAllocator.GetTotalAllocations() +
               ViewportAllocator.GetTotalAllocations();
    }

    const UInt32 GraphicsMemoryManager::GetTotalFreeCount() {
        return VertexbufferAllocator.GetFreeCount() + IndexbufferAllocator.GetFreeCount() +
               LayoutAllocator.GetFreeCount() + ConstantbufferAllocator.GetFreeCount() +
               ShaderAllocator.GetFreeCount() + Texture2DAllocator.GetFreeCount() +
               RenderTexture2DAllocator.GetFreeCount() + DepthTexture2DAllocator.GetFreeCount() +
               ContextAllocator.GetFreeCount() + SwapchainAllocator.GetFreeCount() + ViewportAllocator.GetFreeCount();
    }
}  // namespace Sentinel