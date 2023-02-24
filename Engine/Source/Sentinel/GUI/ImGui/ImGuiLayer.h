#pragma once

namespace Sentinel {
    struct ContextData;
    class Event;

    class ImGuiLayer final {
    public:
        ImGuiLayer(ContextData* context);
        ~ImGuiLayer();

        void OnAttach();
        void OnDetach();

        void Begin();
        void End();
        void OnResize(Event& event);

        void SetDarkThemeColors();

        void BlockEvents(Bool block) { m_BlockEvents = block; }

    private:
        ContextData* m_Context;
        UInt32 m_OnResizeCallbackIndex = 0;
        Bool m_BlockEvents = false;
    };
}  // namespace Sentinel
