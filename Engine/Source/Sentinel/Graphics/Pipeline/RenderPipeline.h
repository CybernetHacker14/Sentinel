#pragma once

#include "Sentinel/Common/Common.h"

namespace Sentinel {
    class IPipelineOperation;

    class RenderPipeline {
    public:
        void AddOperation(IPipelineOperation& operation);
        void RemoveOperation(IPipelineOperation& operation);
        void ExecuteRenderQueue(IPipelineOperation& operation);

    private:
        STL::vector<IPipelineOperation> m_RenderOperationQueue;
    };
}  // namespace Sentinel