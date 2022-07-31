#pragma once

#include "Sentinel/Common/Common.h"

namespace Sentinel {
    class IPipelineOperation {
    public:
        inline void Setup() {
            if (!m_SetupFn) return;
            m_SetupFn();
        }

        inline void Execute() {
            if (!m_ExecuteFn) return;
            m_ExecuteFn();
        }

        inline void Reset() {
            if (!m_ResetFn) return;
            m_ResetFn();
        }

    protected:
        STL::delegate<void()> m_SetupFn;
        STL::delegate<void()> m_ExecuteFn;
        STL::delegate<void()> m_ResetFn;
    };
}  // namespace Sentinel