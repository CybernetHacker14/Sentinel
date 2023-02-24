#pragma once

#include <Sentinel.h>

#include <flecs.h>

namespace Sentinel {
    class Scene;
    class Entity;
}  // namespace Sentinel

namespace Scribe {
    namespace Panel {
        class SceneHierarchyPanel {
        public:
            inline void SetScene(Sentinel::Scene* scene) { m_LoadedScene = scene; }

            void DisplayScenePanel();

        private:
            Sentinel::Bool DisplayNode(Sentinel::Entity* entity);

        private:
            Sentinel::Scene* m_LoadedScene = nullptr;
            Sentinel::Entity* m_SelectedEntity = nullptr;
            Sentinel::Entity* m_MarkedForDelete = nullptr;
        };
    }  // namespace Panel
}  // namespace Scribe