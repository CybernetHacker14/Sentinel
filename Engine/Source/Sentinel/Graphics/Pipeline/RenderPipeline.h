#pragma once

#include "Sentinel/Common/Common.h"
#include "Sentinel/Graphics/Pipeline/IPipelineOperation.h"

namespace Sentinel {
    class RenderPipeline {
    public:
        STL::vector<IPipelineOperation> m_CreateOpQueue;
        STL::vector<IPipelineOperation> m_RenderOpQueue;
        STL::vector<IPipelineOperation> m_UnbindOpQueue;
    };
}  // namespace Sentinel