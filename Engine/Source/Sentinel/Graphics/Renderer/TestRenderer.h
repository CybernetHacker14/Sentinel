#pragma once

#include "Sentinel/Common/Common.h"

namespace Sentinel {
    class Backend;
    class GraphicsMemoryManager;

    class RenderPipeline;
    class IPipelineOperation;

    class TestRenderer {
    public:
        TestRenderer();
        ~TestRenderer();

        void ConstructOperationQueue();

        inline SharedRef<GraphicsMemoryManager> GetMemoryManager() { return m_GFXMemory; }

    private:
        RenderPipeline m_RenderPipeline;
        SharedRef<GraphicsMemoryManager> m_GFXMemory;
    };
}  // namespace Sentinel