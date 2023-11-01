#include "Source/Panels/ContentBrowserPanel.h"
#include "Renderer/ImGuiBase.h"

#include <Sentinel/Graphics/Texture/Texture2DAPI.h>
#include <Sentinel/Logging/Log.h>

#include <imgui.h>
#include <imgui_internal.h>

#include <IconsFontAwesome6.h>
#include <IconsMaterialDesignIcons.h>

#include <sparse_map.h>

namespace Scribe {
    namespace Panel {
        namespace ContentBrowserPanelUtils {
            // This is added because, we want to generate path string labels to be displayed.
            // But the function itself should run once, after the ImGuiLayer.Begin() call is done at the start of ImGui
            // rendering calls
            static Sentinel::Bool s_ComputePathStringsInitFlag = false;

            static const glm::vec2& s_FolderUV0 {0.0f, 0.0f};
            static const glm::vec2& s_FolderUV1 {0.333f, 0.5f};

            static const glm::vec2& s_File_TTFUV0 {0.666f, 0.0f};
            static const glm::vec2& s_File_TTFUV1 {1.0f, 0.5f};

            static const glm::vec2& s_File_HLSLUV0 {0.666f, 0.5f};
            static const glm::vec2& s_File_HLSLUV1 {1.0f, 1.0f};

        }  // namespace ContentBrowserPanelUtils

        ContentBrowserPanel::ContentBrowserPanel(Sentinel::Texture2DData* spriteSheet) : m_SpriteSheetTex(spriteSheet) {
            m_BaseDirectory = Sentinel::Path("Assets");
            m_CurrentSelectedPath = Sentinel::Path("Assets");

            m_CurrentSelectedFolderImmediatePaths = Sentinel::Filesystem::GetImmediatePaths(m_CurrentSelectedPath);
        }

        ContentBrowserPanel::~ContentBrowserPanel() {
            m_CurrentSelectedFolderImmediatePaths.Clear();
        }

        void ContentBrowserPanel::Display() {
            if (!ContentBrowserPanelUtils::s_ComputePathStringsInitFlag) {
                ComputeIconLabelStrings();
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

            ImGuiID popupID = ImGui::GetID("Test");

            if (ImGui::BeginTable(
                    "ContentBrowserTable",
                    1,
                    ImGuiTableFlags_BordersInnerH | ImGuiTableFlags_Resizable | ImGuiTableFlags_NoClip)) {
                ImGui::TableNextRow();
                ImGui::TableSetColumnIndex(0);

                DrawProjectPanel_MenuBar();

                ImGui::TableNextRow();
                ImGui::TableSetColumnIndex(0);

                if (ImGui::BeginChild(
                        "Test",
                        {ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y - 16.0f},
                        false,
                        ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse)) {
                    // Yellow is content region min/max
                    /*{
                        ImVec2 vMin = ImGui::GetWindowContentRegionMin();
                        ImVec2 vMax = ImGui::GetWindowContentRegionMax();

                        vMin.x += ImGui::GetWindowPos().x;
                        vMin.y += ImGui::GetWindowPos().y;
                        vMax.x += ImGui::GetWindowPos().x;
                        vMax.y += ImGui::GetWindowPos().y;

                        ImGui::GetForegroundDrawList()->AddRect(vMin, vMax, IM_COL32(255, 255, 0, 255));
                    }*/

                    ImVec2 outerPadding {15.0f, 15.0f};
                    ImVec2 cellSpacing {15.0f, 8.0f};
                    ImVec2 start = ImGui::GetCursorPos();

                    Sentinel::Float width = ImGui::GetContentRegionAvail().x - (outerPadding.x * 2);

                    Sentinel::UInt32 maxColumns = ST_MAX((Sentinel::UInt32)(width / (64.0f + cellSpacing.x)), 1);
                    start.x += outerPadding.x;
                    start.y += outerPadding.y;

                    Sentinel::UInt32 maxRows =
                        (Sentinel::UInt32)(m_CurrentSelectedFolderImmediatePaths.Size() / maxColumns) + 1;

                    Sentinel::UInt32 fileIndex = 0;

                    Sentinel::Float iconPositionX = 0.0f;
                    Sentinel::Float iconPositionY = 0.0f;

                    for (Sentinel::UInt32 row = 0; row < maxRows; ++row) {
                        for (Sentinel::UInt32 column = 0;
                             column < maxColumns && fileIndex < m_CurrentSelectedFolderImmediatePaths.Size();
                             ++column) {
                            iconPositionX = start.x + (column * 64.0f) + (column * cellSpacing.x);
                            iconPositionY = start.y + (row * 64.0f) + (row * cellSpacing.y) + (row * 20.0f);

                            ImGui::SetCursorPos({iconPositionX, iconPositionY});

                            ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, {0.0f, 0.0f});
                            ImGui::PushStyleColor(ImGuiCol_Button, {0, 0, 0, 0});

                            ImGuiID id =
                                ImGui::GetID(m_CurrentSelectedFolderImmediatePaths[fileIndex].GetAbsolutePath());

                            if (m_CurrentSelectedFolderImmediatePaths[fileIndex].IsFolder()) {
                                if (ImGui::ImageButtonEx(
                                        id,
                                        (ImTextureID)Sentinel::Texture2DAPI::GetNativeResource(m_SpriteSheetTex),
                                        {64.0f, 64.0f},
                                        ContentBrowserPanelUtils::s_FolderUV0,
                                        ContentBrowserPanelUtils::s_FolderUV1,
                                        {0, 0, 0, 0},
                                        {1, 1, 1, 1},
                                        ImGuiButtonFlags_MouseButtonLeft)) {
                                    m_CurrentSelectedPath = m_CurrentSelectedFolderImmediatePaths[fileIndex];
                                    m_CurrentSelectedFolderImmediatePaths =
                                        Sentinel::Filesystem::GetImmediatePaths(m_CurrentSelectedPath);
                                    ComputeIconLabelStrings();
                                }
                                if (ImGui::IsItemHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Right)) {
                                    ImGui::OpenPopupEx(popupID, false);
                                }
                            } else {
                                ImGui::ImageButton(
                                    "TestFile",
                                    (ImTextureID)Sentinel::Texture2DAPI::GetNativeResource(m_SpriteSheetTex),
                                    {64.0f, 64.0f},
                                    ContentBrowserPanelUtils::s_File_TTFUV0,
                                    ContentBrowserPanelUtils::s_File_TTFUV1);
                                if (ImGui::IsItemHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Right)) {
                                    ImGui::OpenPopupEx(popupID, false);
                                }
                            }

                            ImGui::PopStyleColor();
                            ImGui::PopStyleVar();

                            Sentinel::Float size = GImGui->FontSize;
                            GImGui->FontSize = 15.0f;

                            Sentinel::Float offset =
                                (64.0f -
                                 ImGui::CalcTextSize(m_CurrentSelectedFolderImmediatePathStrings[fileIndex].C_Str())
                                     .x) /
                                2.0f;

                            ImGui::SetCursorPos({iconPositionX + offset, iconPositionY + 67.0f});
                            // ImGui::BeginChild("Test", {64.0f, textHeight}, true);
                            ImGui::Text(m_CurrentSelectedFolderImmediatePathStrings[fileIndex].C_Str());
                            GImGui->FontSize = size;
                            // ImGui::EndChild();

                            ++fileIndex;
                        }
                    }

                    ImGui::EndChild();
                }

                ImGui::TableNextRow();
                ImGui::TableSetColumnIndex(0);
                ImGui::Text("%s", m_CurrentSelectedPath.GetAbsolutePath());

                ImGui::EndTable();

                if (ImGui::BeginPopupEx(
                        popupID,
                        ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove)) {
                    bool deletePress = ImGui::MenuItem("Delete");
                    bool cutPress = ImGui::MenuItem("Cut");
                    bool backPress = ImGui::MenuItem("Back");
                    if (deletePress) {
                        ST_TERMINAL_INFO("Delete clicked");
                    } else if (cutPress) {
                        ST_TERMINAL_INFO("Cut clicked");
                    } else if (backPress) {
                        if (strcmp(m_CurrentSelectedPath.GetFilenameWithExtension(), "Assets") != 0) {
                            Sentinel::Char buffer[260];
                            Sentinel::Filesystem::GetParentPath(m_CurrentSelectedPath, buffer);
                            m_CurrentSelectedPath = Sentinel::Path(buffer);
                            m_CurrentSelectedFolderImmediatePaths =
                                Sentinel::Filesystem::GetImmediatePaths(m_CurrentSelectedPath);
                            ComputeIconLabelStrings();
                        }
                    }
                    ImGui::EndPopup();
                }
            }
        }

        void ContentBrowserPanel::DrawProjectPanel_MenuBar() {
            if (ImGui::BeginTable(
                    "ContentBrowserTable_MenuBar",
                    2,
                    ImGuiTableFlags_BordersInnerV | ImGuiTableFlags_NoPadOuterX | ImGuiTableFlags_SizingFixedFit)) {
                ImGui::TableNextRow();
                ImGui::TableSetColumnIndex(0);

                {
                    if (ImGui::Button("Import")) {}
                    ImGui::SameLine();
                    if (ImGui::Button("Delete")) {}
                }

                ImGui::TableSetColumnIndex(1);

                {
                    ImGui::Text(ICON_FA_DATABASE "  /");
                    ImGui::SameLine();
                    if (ImGui::Button(m_CurrentSelectedPath.GetFilenameWithExtension())) {}
                }

                ImGui::EndTable();
            }
        }

        void ContentBrowserPanel::ComputeIconLabelStrings() {
            m_CurrentSelectedFolderImmediatePathStrings.Clear();

            Sentinel::Float size = GImGui->FontSize;
            GImGui->FontSize = 15.0f;

            for (Sentinel::UInt32 count = 0; count < m_CurrentSelectedFolderImmediatePaths.Size(); ++count) {
                std::string buf(m_CurrentSelectedFolderImmediatePaths[count].GetFilenameWithExtension());
                std::string temp = buf;
                while (ImGui::CalcTextSize(temp.c_str()).x > 50.0f) temp.pop_back();
                if (temp != buf) temp += "...";
                m_CurrentSelectedFolderImmediatePathStrings.Emplace_Back(temp.c_str());
            }

            GImGui->FontSize = size;
        }
    }  // namespace Panel
}  // namespace Scribe