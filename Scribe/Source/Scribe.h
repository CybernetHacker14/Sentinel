#pragma once

#include <Sentinel.h>

namespace Sentinel {
    class ImGuiLayer;
}

namespace Scribe {
    namespace Rendering {
        class ScribeRenderer;
        class ScribeImGuiBase;
    }  // namespace Rendering
}  // namespace Scribe

namespace Scribe {
    class Scribe final: public Sentinel::Application {
    public:
        Scribe();
        ~Scribe();

    private:
        void Run();

    private:
        void OnWindowClose(Sentinel::Event& event);

    private:
        Sentinel::UInt32 m_CloseIndex = 0;

        Sentinel::Bool m_Running = true;
        Sentinel::Bool m_Minimized = false;

        Sentinel::ImGuiLayer* m_ImGuiLayer;
        Rendering::ScribeRenderer* m_BaseRenderer;
        Rendering::ScribeImGuiBase* m_ImGuiBase;
    };
}  // namespace Scribe

Sentinel::Application* Sentinel::CreateApplication() {
    return new Scribe::Scribe;
}