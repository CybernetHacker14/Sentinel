#include "stpch.h"
#include "Sentinel/Graphics/Pipeline/ResourceInitOperation.h"
#include "Sentinel/Graphics/Pipeline/IPipelineOperation.h"

#include "Sentinel/Graphics/Common/GraphicsMemoryManager.h"

namespace Sentinel {
    ResourceInitOperation::ResourceInitOperation(SharedRef<GraphicsMemoryManager> memoryHandle) {}

    void ResourceInitOperation::Setup() {}

    void ResourceInitOperation::Execute() {}

    void ResourceInitOperation::Reset() {}
}  // namespace Sentinel