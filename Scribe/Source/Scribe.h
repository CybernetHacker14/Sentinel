#pragma once

#include <Sentinel.h>

#include <Sentinel/ECS/Entity.h>

namespace Sentinel {
    class ImGuiLayer;
    struct Scene;
    struct Entity;
}  // namespace Sentinel

namespace Scribe {
    namespace Rendering {
        class EditorRenderer;
        class SceneRenderer;
        class ImGuiBase;
    }  // namespace Rendering

}  // namespace Scribe

namespace Scribe {
    class Scribe final: public Sentinel::Application {
    public:
        Scribe();
        ~Scribe();

    private:
        void Run() override;

    private:
        Sentinel::Bool OnWindowClose(Sentinel::EventType type, Sentinel::EventData data, void* listener);

    private:
        Sentinel::UInt16 m_CloseIndex = 0;

        Sentinel::Bool m_Running = true;
        Sentinel::Bool m_Minimized = false;

        Sentinel::ImGuiLayer* m_ImGuiLayer;
        Rendering::EditorRenderer* m_BaseRenderer;
        Rendering::SceneRenderer* m_SceneRenderer;
        Rendering::ImGuiBase* m_ImGuiBase;
    };
}  // namespace Scribe

Sentinel::Application* Sentinel::CreateApplication() {
    return new Scribe::Scribe;
}