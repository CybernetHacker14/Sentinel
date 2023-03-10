#pragma once

#include <Sentinel.h>

namespace Sentinel {
    class ImGuiLayer;
    struct Scene;
    struct Entity;
}  // namespace Sentinel

namespace Scribe {
    namespace Rendering {
        class ScribeRenderer;
        class ScribeImGuiBase;
    }  // namespace Rendering

    namespace Panel {
        class SceneHierarchyPanel;
    }
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

    private:
        Sentinel::Scene* m_TestScene;
        Sentinel::Entity* m_Entity1;
        Sentinel::Entity* m_Entity2;
        Sentinel::Entity* m_Entity3;
        Sentinel::Entity* m_Entity4;
        Sentinel::Entity* m_Entity5;
        Sentinel::Entity* m_Entity6;
        Sentinel::Entity* m_Entity7;
    };
}  // namespace Scribe

Sentinel::Application* Sentinel::CreateApplication() {
    return new Scribe::Scribe;
}