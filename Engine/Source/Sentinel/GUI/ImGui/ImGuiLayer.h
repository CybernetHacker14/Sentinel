#pragma once

#include "Sentinel/Common/Core/DataTypes.h"
#include "Sentinel/Events/Event.h"

namespace Sentinel {
    struct ContextData;

    class ImGuiLayer final {
    public:
        ImGuiLayer(ContextData* context);
        ~ImGuiLayer();

        void OnAttach();
        void OnDetach();

        void Begin();
        void End();
        Bool OnResize(EventType type, EventData data, void* object);

        void SetColors();

        void BlockEvents(Bool block) { m_BlockEvents = block; }

    private:
        ContextData* m_Context;
        Int16 m_OnResizeCallbackIndex = 0;
        Bool m_BlockEvents = false;
    };
}  // namespace Sentinel
