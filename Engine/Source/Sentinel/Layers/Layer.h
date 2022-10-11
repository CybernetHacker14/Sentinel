#pragma once

#include "Sentinel/Common/Common.h"

namespace Sentinel {
    class Layer {
    public:
        inline Layer(const STL::string& debugName = "Layer") : m_DebugName(debugName) {}

        inline void OnAttach() {
            if (!m_AttachFunction) return;
            m_AttachFunction();
        };

        inline void OnDetach() {
            if (!m_DetachFunction) return;
            m_DetachFunction();
        };

        inline void OnUpdate() {
            if (!m_UpdateFunction) return;
            m_UpdateFunction();
        };

        inline void OnRender() {
            if (!m_RenderFunction) return;
            m_RenderFunction();
        };

        inline void OnImGuiRender() {
            if (!m_ImGuiRenderFunction) return;
            m_ImGuiRenderFunction();
        };

        inline void OnPostRender() {
            if (!m_PostRenderFunction) return;
            m_PostRenderFunction();
        }

        const STL::string& GetName() const { return m_DebugName; }

    protected:
        STL::delegate<void()> m_AttachFunction;
        STL::delegate<void()> m_DetachFunction;
        STL::delegate<void()> m_UpdateFunction;
        STL::delegate<void()> m_RenderFunction;
        STL::delegate<void()> m_ImGuiRenderFunction;
        STL::delegate<void()> m_PostRenderFunction;

    protected:
        STL::string m_DebugName;
    };
}  // namespace Sentinel
