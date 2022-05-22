#include "stpch.h"
#include "Sentinel/Memory/MemoryManager.h"

namespace Sentinel {
    MemoryManager::MemoryManager() { Texture2DAllocator.AllocateMemoryBlock(16); }

    MemoryManager::~MemoryManager() {
        Texture2DAllocator.DeleteAll();
        Texture2DAllocator.DeallocateMemoryBlock();
    }
}  // namespace Sentinel