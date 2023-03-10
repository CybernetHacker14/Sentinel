#include "Panels/SceneHierarchyPanel.h"

#include <Sentinel/ECS/Scene.h>
#include <Sentinel/ECS/Components.h>

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

            flecs::world* scene = m_LoadedScene->GetNativeScene();

            /*ecs_iter_t it;
            auto q = scene->query<Sentinel::TransformComponent>();

            ecs_iter_poly(scene->c_ptr(), scene->c_ptr(), &it, NULL);
            while (ecs_iter_next(&it)) {
                for (int i = 0; i < it.count; i++) {
                    flecs::entity e(scene->c_ptr(), it.entities[i]);
                    bool opened = ImGui::TreeNodeEx((void*)(Sentinel::UInt64)e, flags, e.name().c_str());
                }
            }*/

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
