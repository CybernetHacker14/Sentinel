#pragma once

#include <Sentinel.h>

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
            void DrawEntityNode(Sentinel::Entity* entity);

        private:
            Sentinel::Scene* m_LoadedScene = nullptr;
        };
    }  // namespace Panel
}  // namespace Scribe