#pragma once

#include <Sentinel.h>
#include <Sentinel/ECS/Entity.h>

namespace Sentinel {
    struct Scene;
}  // namespace Sentinel

namespace Scribe {
    namespace Panel {
        class SceneHierarchyPanel {
        public:
            inline void SetScene(Sentinel::Scene* scene) { m_LoadedScene = scene; }

            void DisplayScenePanel();

        private:
            void DisplayNode(Sentinel::Entity entity);

        private:
            Sentinel::Scene* m_LoadedScene = nullptr;
            Sentinel::Entity* m_SelectedEntity = nullptr;
            Sentinel::Entity m_MarkedForDelete;
        };
    }  // namespace Panel
}  // namespace Scribe