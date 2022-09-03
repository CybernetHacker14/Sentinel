#pragma once

#include "Sentinel/Layers/Layer.h"

namespace Sentinel {
    struct ContextData;
    class Event;

    class ImGuiLayer final: public Layer {
    public:
        ImGuiLayer(ContextData* context);
        ~ImGuiLayer();

        void OnAttach();
        void OnDetach();

        void Begin();
        void End();
        void OnResize(Event& event);

    private:
        ContextData* m_Context;
        UInt32 m_OnResizeCallbackIndex = 0;
    };
}  // namespace Sentinel
