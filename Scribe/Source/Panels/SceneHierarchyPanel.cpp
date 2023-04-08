#include "Panels/SceneHierarchyPanel.h"

#include <Sentinel/ECS/Scene.h>
#include <Sentinel/ECS/Components.h>

#include <imgui.h>
#include <imgui_internal.h>

namespace Scribe {
    namespace Panel {
        void SceneHierarchyPanel::DisplayScenePanel() {
            if (m_LoadedScene == nullptr) return;

            char buf[128];
            sprintf_s(buf, 128, "%s###HierarchyPanel", m_LoadedScene->GetName());
            ImGui::Begin(buf);

            flecs::world* scene = m_LoadedScene->GetNativeScene();

            scene->component<Sentinel::TransformComponent>();
            scene->each<Sentinel::TransformComponent>([&](flecs::entity e, Sentinel::TransformComponent& transform) {
                if (!m_LoadedScene->registry[e]->HasParent()) { DisplayNode(m_LoadedScene->registry[e]); }
            });

            ImGui::End();

            if (m_MarkedForDelete != nullptr) {
                m_LoadedScene->DeleteEntity(m_MarkedForDelete);
                m_MarkedForDelete = nullptr;
            }
        }

        void SceneHierarchyPanel::DisplayNode(Sentinel::Entity* entity) {
            ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_SpanAvailWidth;
            flags |= entity->HasChildren() ? ImGuiTreeNodeFlags_OpenOnArrow : ImGuiTreeNodeFlags_Leaf;

            flecs::entity* e = entity->GetNative();
            bool opened = ImGui::TreeNodeEx((void*)(Sentinel::UInt64)*e, flags, e->name().c_str());
            // if (!opened && ImGui::IsItemClicked(0)) m_SelectedEntity = entity;

            if (ImGui::BeginPopupContextItem()) {
                if (ImGui::MenuItem("Delete Entity")) { m_MarkedForDelete = entity; }
                ImGui::EndPopup();
            }

            if (opened) {
                e->children([&](flecs::entity e) {
                    DisplayNode(m_LoadedScene->registry[e]);
                });
                ImGui::TreePop();
            }
        }
    }  // namespace Panel
}  // namespace Scribe
