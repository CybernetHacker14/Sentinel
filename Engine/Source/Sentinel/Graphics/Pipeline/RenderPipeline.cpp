#include "stpch.h"
#include "Sentinel/Graphics/Pipeline/RenderPipeline.h"
#include "Sentinel/Graphics/Pipeline/IPipelineOperation.h"

namespace Sentinel {
    void RenderPipeline::AddOperation(IPipelineOperation& operation) { m_RenderOperationQueue.emplace_back(operation); }

    void RenderPipeline::RemoveOperation(IPipelineOperation& operation) {
        STL::remove(m_RenderOperationQueue.begin(), m_RenderOperationQueue.end(), operation);
    }

    void RenderPipeline::ExecuteRenderQueue(IPipelineOperation& operation) {
        for (auto& operation: m_RenderOperationQueue) {
            operation.Setup();
            operation.Execute();
            operation.Reset();
        }
    }
}  // namespace Sentinel