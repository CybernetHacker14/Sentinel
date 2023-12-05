#include "Source/Panels/ContentBrowserPanel.h"
#include "Renderer/ImGuiBase.h"

#include <Sentinel/Graphics/Texture/Texture2DAPI.h>
#include <Sentinel/Common/Strings/StringView.h>
#include <Sentinel/Logging/Log.h>

#include <imgui.h>
#include <imgui_internal.h>

#include <IconsFontAwesome6.h>
#include <IconsMaterialDesignIcons.h>

#include <string>

namespace Scribe {
    namespace Panel {
        namespace ContentBrowserPanelUtils {
            // This is added because, we want to generate path string labels to be displayed.
            // But the function itself should run once, after the ImGuiLayer.Begin() call is done at the start of ImGui
            // rendering calls
            static Sentinel::Bool s_ComputePathStringsInitFlag = false;

            static Sentinel::Float s_IconSize = 64.0f;

            static const ImVec2& s_FolderUV0 {0.00f, 0.0f};
            static const ImVec2& s_FolderUV1 {0.25f, 0.5f};

            static const ImVec2& s_File_NAUV0 {0.00f, 0.5f};
            static const ImVec2& s_File_NAUV1 {0.25f, 1.0f};

            static const ImVec2& s_File_TTFUV0 {0.50f, 0.0f};
            static const ImVec2& s_File_TTFUV1 {0.75f, 0.5f};

            static const ImVec2& s_File_HLSLUV0 {0.50f, 0.5f};
            static const ImVec2& s_File_HLSLUV1 {0.75f, 1.0f};

            static const ImVec2& s_OuterPadding {15.0f, 15.0f};
            static const ImVec2& s_CellSpacing {15.0f, 8.0f};

            static const std::pair<ImVec2, ImVec2> s_FileIconUVs[] = {
                {s_File_NAUV0, s_File_NAUV1}, {s_File_TTFUV0, s_File_TTFUV1}, {s_File_HLSLUV0, s_File_HLSLUV1}};

        }  // namespace ContentBrowserPanelUtils

        ContentBrowserPanel::ContentBrowserPanel(Sentinel::Texture2DData* spriteSheet) : m_SpriteSheetTex(spriteSheet) {
            m_BaseDirectory = Sentinel::Path("Assets");
            m_CurrentSelectedPath = Sentinel::Path("Assets");

            m_CurrentSelectedFolderParentHierarchyPaths.Emplace_Back(m_CurrentSelectedPath);
        }

        ContentBrowserPanel::~ContentBrowserPanel() {
            m_CurrentSelectedFolderImmediatePaths.Clear();
        }

        void ContentBrowserPanel::Display() {
            if (!ContentBrowserPanelUtils::s_ComputePathStringsInitFlag) {
                UpdateCurrentSelectedFolderCachedData();
                m_PopupWindowID = ImGui::GetID("ContentBrowserTable_RCContextMenu");
                ContentBrowserPanelUtils::s_ComputePathStringsInitFlag = true;
            }

            ImGui::Begin(
                ICON_FA_FOLDER_CLOSED " Content Browser",
                (Sentinel::Bool*)0,
                ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollWithMouse);

            ImGui::BeginTable(
                "ContentBrowserTable_Outermost", 1, ImGuiTableFlags_BordersV, ImGui::GetContentRegionAvail(), 200.0f);

            ImGui::TableNextRow();

            DrawProjectPanel();

            ImGui::EndTable();

            ImGui::ShowMetricsWindow();

            ImGui::End();
        }

        void ContentBrowserPanel::DrawProjectPanel() {
            ImGui::TableSetColumnIndex(0);

            if (ImGui::BeginTable(
                    "ContentBrowserTable",
                    1,
                    ImGuiTableFlags_BordersInnerH | ImGuiTableFlags_Resizable | ImGuiTableFlags_NoClip)) {
                ImGui::TableNextRow();
                ImGui::TableSetColumnIndex(0);

                DrawProjectPanel_MenuBar();

                ImGui::TableNextRow();
                ImGui::TableSetColumnIndex(0);

                DrawProjectPanel_MiddleSection();

                ImGui::TableNextRow();
                ImGui::TableSetColumnIndex(0);

                DrawProjectPanel_BottomBar();

                ImGui::EndTable();
            }
        }

        void ContentBrowserPanel::DrawProjectPanel_MenuBar() {
            if (ImGui::BeginTable(
                    "ContentBrowserTable_MenuBar",
                    3,
                    ImGuiTableFlags_BordersInnerV | ImGuiTableFlags_NoPadOuterX | ImGuiTableFlags_SizingFixedFit)) {
                ImGui::TableNextRow();
                ImGui::TableSetColumnIndex(0);
                {
                    if (ImGui::Button(ICON_FA_FILE_CIRCLE_PLUS)) {}
                    ImGui::SameLine();
                    if (ImGui::Button(ICON_FA_TRASH_CAN)) {}
                    ImGui::SameLine();
                    if (ImGui::Button(ICON_FA_ARROW_LEFT)) { GoToPreviousFolder(); }
                }

                ImGui::TableSetColumnIndex(1);
                {
                    static Sentinel::Char buf[256];

                    ImGui::PushItemWidth(200);
                    if (ImGui::InputTextWithHint(
                            "###ContentBrowserTable_SearchBar",
                            ICON_FA_MAGNIFYING_GLASS "  Search",
                            buf,
                            IM_ARRAYSIZE(buf))) {}
                    ImGui::PopItemWidth();
                }

                ImGui::TableSetColumnIndex(2);
                {
                    ImGui::Text(ICON_FA_DATABASE "  /");
                    ImGui::SameLine();

                    for (Sentinel::UInt32 i = 0; i < m_CurrentSelectedFolderParentHierarchyPaths.Size(); ++i) {
                        if (ImGui::Button(m_CurrentSelectedFolderParentHierarchyPaths[i].GetFilenameWithExtension())) {
                            Sentinel::UInt32 popCount = m_CurrentSelectedFolderParentHierarchyPaths.Size() - i;
                            m_CurrentSelectedPath = m_CurrentSelectedFolderParentHierarchyPaths[i];
                            UpdateCurrentSelectedFolderCachedData();
                            for (Sentinel::UInt32 j = 1; j < popCount; ++j)
                                m_CurrentSelectedFolderParentHierarchyPaths.Pop_Back();
                        }
                        ImGui::SameLine();
                        ImGui::Text(ICON_FA_GREATER_THAN);
                        ImGui::SameLine();
                    }
                }

                ImGui::EndTable();
            }
        }

        void ContentBrowserPanel::DrawProjectPanel_MiddleSection() {
            if (ImGui::BeginChild(
                    "ContentBrowserTable_MiddleSection",
                    {ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y - 21.0f},
                    false,
                    ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse)) {
                ImVec2 start = ImGui::GetCursorPos();

                Sentinel::Float width =
                    ImGui::GetContentRegionAvail().x - (ContentBrowserPanelUtils::s_OuterPadding.x * 2);

                Sentinel::UInt32 maxColumns = ST_MAX(
                    (Sentinel::UInt32)(
                        width / (ContentBrowserPanelUtils::s_IconSize + ContentBrowserPanelUtils::s_CellSpacing.x)),
                    1);
                start.x += ContentBrowserPanelUtils::s_OuterPadding.x;
                start.y += ContentBrowserPanelUtils::s_OuterPadding.y;

                Sentinel::UInt32 maxRows =
                    (Sentinel::UInt32)(m_CurrentSelectedFolderImmediatePaths.Size() / maxColumns) + 1;

                Sentinel::UInt32 fileIndex = 0;

                Sentinel::Float iconPositionX = 0.0f;
                Sentinel::Float iconPositionY = 0.0f;

                for (Sentinel::UInt32 row = 0; row < maxRows; ++row) {
                    for (Sentinel::UInt32 column = 0;
                         column < maxColumns && fileIndex < m_CurrentSelectedFolderImmediatePaths.Size();
                         ++column) {
                        iconPositionX = start.x + (column * ContentBrowserPanelUtils::s_IconSize) +
                                        (column * ContentBrowserPanelUtils::s_CellSpacing.x);
                        iconPositionY = start.y + (row * ContentBrowserPanelUtils::s_IconSize) +
                                        (row * ContentBrowserPanelUtils::s_CellSpacing.y) + (row * 20.0f);

                        ImGui::SetCursorPos({iconPositionX, iconPositionY});

                        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, {0.0f, 0.0f});
                        ImGui::PushStyleColor(ImGuiCol_Button, {0, 0, 0, 0});

                        ImGuiID id = ImGui::GetID(m_CurrentSelectedFolderImmediatePaths[fileIndex].GetAbsolutePath());

                        if (m_CurrentSelectedFolderImmediatePaths[fileIndex].IsFolder()) {
                            ImGui::ImageButtonEx(
                                id,
                                (ImTextureID)Sentinel::Texture2DAPI::GetNativeResource(m_SpriteSheetTex),
                                {ContentBrowserPanelUtils::s_IconSize, ContentBrowserPanelUtils::s_IconSize},
                                ContentBrowserPanelUtils::s_FolderUV0,
                                ContentBrowserPanelUtils::s_FolderUV1,
                                {0, 0, 0, 0},
                                {1, 1, 1, 1});

                            if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left)) {
                                m_CurrentSelectedPath = m_CurrentSelectedFolderImmediatePaths[fileIndex];
                                m_CurrentSelectedFolderParentHierarchyPaths.Emplace_Back(m_CurrentSelectedPath);
                                UpdateCurrentSelectedFolderCachedData();
                            }
                            if (ImGui::IsItemHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Right)) {
                                ImGui::OpenPopupEx(m_PopupWindowID, false);
                            }
                        } else {
                            ImGui::ImageButtonEx(
                                id,
                                (ImTextureID)Sentinel::Texture2DAPI::GetNativeResource(m_SpriteSheetTex),
                                {ContentBrowserPanelUtils::s_IconSize, ContentBrowserPanelUtils::s_IconSize},
                                ContentBrowserPanelUtils::s_FileIconUVs
                                    [(Sentinel::UInt8)m_CurrentSelectedFolderImmediatePaths[fileIndex].GetFileType()]
                                        .first,
                                ContentBrowserPanelUtils::s_FileIconUVs
                                    [(Sentinel::UInt8)m_CurrentSelectedFolderImmediatePaths[fileIndex].GetFileType()]
                                        .second,
                                {0, 0, 0, 0},
                                {1, 1, 1, 1});

                            if (ImGui::IsItemHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Right)) {
                                ImGui::OpenPopupEx(m_PopupWindowID, false);
                            }
                        }

                        ImGui::PopStyleColor();
                        ImGui::PopStyleVar();

                        Sentinel::Float size = GImGui->FontSize;
                        GImGui->FontSize = (15.0f * ContentBrowserPanelUtils::s_IconSize) / 64.0f;

                        Sentinel::Float offset =
                            (ContentBrowserPanelUtils::s_IconSize -
                             ImGui::CalcTextSize(m_CurrentSelectedFolderImmediatePathStrings[fileIndex].C_Str()).x) /
                            2.0f;

                        ImGui::SetCursorPos(
                            {iconPositionX + offset, iconPositionY + ContentBrowserPanelUtils::s_IconSize + 3.0f});
                        // ImGui::BeginChild("Test", {64.0f, textHeight}, true);
                        ImGui::Text(m_CurrentSelectedFolderImmediatePathStrings[fileIndex].C_Str());
                        GImGui->FontSize = size;
                        // ImGui::EndChild();

                        ++fileIndex;
                    }
                }

                DrawProjectPanel_RightClickContextMenu();

                ImGui::EndChild();
            }
        }

        void ContentBrowserPanel::DrawProjectPanel_RightClickContextMenu() {
            if (ImGui::BeginPopupEx(
                    m_PopupWindowID,
                    ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove)) {
                bool deletePress = ImGui::MenuItem("Delete");
                bool cutPress = ImGui::MenuItem("Cut");
                bool backPress = ImGui::MenuItem("Back");
                if (deletePress) {
                    ST_TERMINAL_INFO("Delete clicked");
                } else if (cutPress) {
                    ST_TERMINAL_INFO("Cut clicked");
                } else if (backPress) {
                    GoToPreviousFolder();
                }
                ImGui::EndPopup();
            }
        }

        void ContentBrowserPanel::DrawProjectPanel_BottomBar() {
            if (ImGui::BeginTable(
                    "ContentBrowserTable_BottomBar",
                    2,
                    ImGuiTableFlags_BordersInnerV | ImGuiTableFlags_NoPadOuterX | ImGuiTableFlags_SizingFixedFit)) {
                ImGui::TableNextRow();
                ImGui::TableSetColumnIndex(0);

                ImGui::PushItemWidth(100.0f);
                ImGui::SliderFloat(
                    "###ContentBrowserPanel_IconSizeSlider", &ContentBrowserPanelUtils::s_IconSize, 40.0f, 64.0f, "");
                ImGui::PopItemWidth();

                ImGui::TableSetColumnIndex(1);
                ImGui::Text("%s", m_CurrentSelectedPath.GetAbsolutePath());

                ImGui::EndTable();
            }
        }

        void ContentBrowserPanel::UpdateCurrentSelectedFolderCachedData() {
            m_CurrentSelectedFolderImmediatePaths = Sentinel::Filesystem::GetImmediatePaths(m_CurrentSelectedPath);
            m_CurrentSelectedFolderButtonStates.Clear();
            for (Sentinel::UInt32 count = 0; count < m_CurrentSelectedFolderImmediatePaths.Size(); ++count)
                m_CurrentSelectedFolderButtonStates.Emplace_Back(false);
            ComputeIconLabelStrings();
        }

        void ContentBrowserPanel::ComputeIconLabelStrings() {
            m_CurrentSelectedFolderImmediatePathStrings.Clear();

            Sentinel::Float size = GImGui->FontSize;
            GImGui->FontSize = (15.0f * ContentBrowserPanelUtils::s_IconSize) / 64.0f;

            for (Sentinel::UInt32 count = 0; count < m_CurrentSelectedFolderImmediatePaths.Size(); ++count) {
                std::string buf(m_CurrentSelectedFolderImmediatePaths[count].GetFilenameWithExtension());
                std::string temp = buf;
                while (ImGui::CalcTextSize(temp.c_str()).x > 50.0f) temp.pop_back();
                if (temp != buf) temp += "...";
                m_CurrentSelectedFolderImmediatePathStrings.Emplace_Back(temp.c_str());
            }

            GImGui->FontSize = size;
        }

        void ContentBrowserPanel::GoToPreviousFolder() {
            if (strcmp(m_CurrentSelectedPath.GetFilenameWithExtension(), "Assets") != 0) {
                Sentinel::Char buffer[260];
                Sentinel::Filesystem::GetParentPath(m_CurrentSelectedPath, buffer);
                m_CurrentSelectedPath = Sentinel::Path(buffer);
                UpdateCurrentSelectedFolderCachedData();
                m_CurrentSelectedFolderParentHierarchyPaths.Pop_Back();
            }
        }
    }  // namespace Panel
}  // namespace Scribe