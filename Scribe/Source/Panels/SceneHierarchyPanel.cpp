#include "Panels/SceneHierarchyPanel.h"

#include <Sentinel/ECS/Scene.h>

#include <imgui.h>
#include <imgui_internal.h>

namespace Scribe {
    namespace Panel {
        void SceneHierarchyPanel::DisplayScenePanel() {
            ImGui::Begin("Scene Hierarchy");
            if (m_LoadedScene == nullptr) {
                ImGui::End();
                return;
            }

            for (Sentinel::Entity* entity: m_LoadedScene->entities) { DrawEntityNode(entity); }

            ImGui::End();
        }

        void SceneHierarchyPanel::DrawEntityNode(Sentinel::Entity* entity) {
            ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow;
            flags |= ImGuiTreeNodeFlags_SpanAvailWidth;
            bool opened = ImGui::TreeNodeEx((void*)(uint64_t)entity, flags, entity->GetNative().name().c_str());
        }
    }  // namespace Panel
}  // namespace Scribe
