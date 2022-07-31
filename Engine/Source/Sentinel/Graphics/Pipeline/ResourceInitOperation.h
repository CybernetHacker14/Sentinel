#pragma once

#include "Sentinel/Common/Common.h"

namespace Sentinel {
    class IPipelineOperation;
    class GraphicsMemoryManager;

    class ResourceInitOperation final: public IPipelineOperation {
    public:
        ResourceInitOperation(SharedRef<GraphicsMemoryManager> memoryHandle);

    private:
        void Setup();
        void Execute();
        void Reset();
    };
}  // namespace Sentinel