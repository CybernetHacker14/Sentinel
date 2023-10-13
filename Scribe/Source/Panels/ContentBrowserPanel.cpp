#include "ContentBrowserPanel.h"

#include <imgui.h>
#include <imgui_internal.h>

#include <IconsFontAwesome6.h>
#include <IconsMaterialDesignIcons.h>

namespace Scribe {
    namespace Panel {
        ContentBrowserPanel::ContentBrowserPanel() {
            m_BaseDirectory = Sentinel::Path("Assets");
            m_CurrentSelectedPath = Sentinel::Path("Assets");
        }

        void ContentBrowserPanel::Display() {
            ImGui::Begin(
                ICON_FA_FOLDER_CLOSED " Content Browser",
                (Sentinel::Bool*)0,
                ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoCollapse);

            ImGui::BeginTable(
                "ContentBrowserTable",
                2,
                ImGuiTableFlags_BordersV | ImGuiTableFlags_Resizable | ImGuiTableFlags_ScrollY,
                ImGui::GetContentRegionAvail(),
                200.0f);

            // Display left project tree
            DrawLeftProjectPanel();

            if (GImGui->CurrentTable != nullptr) ImGui::EndTable();

            ImGui::End();

            ImGui::ShowMetricsWindow();
        }

        void ContentBrowserPanel::DrawLeftProjectPanel() {
            if (GImGui->CurrentTable == nullptr) return;

            ImGui::TableNextRow();
            ImGui::TableSetColumnIndex(0);
            Sentinel::Bool opened = ImGui::TreeNodeEx(
                "ContentBrowserTable_LeftProjectPanel_StartNode",
                ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_OpenOnArrow,
                "%s %s",
                (m_BaseDirectory.IsFolderEmpty() ? ICON_MDI_FOLDER_OUTLINE : ICON_MDI_FOLDER),
                m_BaseDirectory.GetFilenameWithExtension());

            // Probably a hack
            GImGui->CurrentTable->MinColumnWidth = 200.0f;

            if (opened) {
                DrawLeftNodeTreeRecursive(m_BaseDirectory);
                ImGui::TreePop();
            }
        }

        void ContentBrowserPanel::DrawLeftNodeTreeRecursive(const Sentinel::Path& path) {
            Sentinel::Vector<Sentinel::Path> allSelectedPaths = Sentinel::Filesystem::GetImmediateSubfolders(path);
            for (Sentinel::UInt32 i = 0; i < allSelectedPaths.Size(); i++) {
                ImGui::TableNextRow();
                ImGui::TableSetColumnIndex(0);

                char buf[256];
                sprintf_s(
                    buf,
                    256,
                    "ContentBrowserTable_LeftProjectPanel_%s",
                    allSelectedPaths[i].GetFilenameWithExtension());

                Sentinel::Bool opened = ImGui::TreeNodeEx(
                    buf,
                    !Sentinel::Filesystem::HasSubfolders(allSelectedPaths[i]) ? ImGuiTreeNodeFlags_Leaf
                                                                              : ImGuiTreeNodeFlags_OpenOnArrow,
                    "%s %s",
                    (allSelectedPaths[i].IsFolderEmpty() ? ICON_MDI_FOLDER_OUTLINE : ICON_MDI_FOLDER),
                    allSelectedPaths[i].GetFilenameWithExtension());

                if (!Sentinel::Filesystem::HasSubfolders(allSelectedPaths[i])) {
                    ImGui::TreePop();

                } else if (opened) {
                    DrawLeftNodeTreeRecursive(allSelectedPaths[i]);
                    ImGui::TreePop();
                }
            }
        }
    }  // namespace Panel
}  // namespace Scribe