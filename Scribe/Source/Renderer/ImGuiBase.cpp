#include "Renderer/ImGuiBase.h"
#include "Window/ScribeWindow.h"

#include "Panels/SceneHierarchyPanel.h"
#include "Panels/ContentBrowserPanel.h"

#include <Sentinel/Graphics/Device/ContextAPI.h>
#include <Sentinel/Graphics/Texture/Texture2DAPI.h>

#include <imgui.h>
#include <sparse_map.h>

#include <IconsFontAwesome6.h>
#include <IconsFontAwesome6Brands.h>
#include <IconsMaterialDesignIcons.h>

#include "Icons/Spritesheets/TitleBar_SpriteSheet.inl"
#include "Icons/Spritesheets/ContentBrowserSpriteSheet.inl"

namespace Scribe {
    namespace Rendering {
        namespace ImGuiBaseUtils {
            static const Sentinel::Float s_TitleBarHeight = 64.0f;

            static const glm::vec2& s_IconSize {s_TitleBarHeight, s_TitleBarHeight};
            static const glm::vec2& s_IconUV0 {0, 0};
            static const glm::vec2& s_IconUV1 {0.333f, 0.5f};

            static const glm::vec2& s_ControlBtnSize {20, 20};
            static const glm::vec2& s_MinimizeUV0 {0.5f, 0.0f};
            static const glm::vec2& s_MinimizeUV1 {0.666f, 0.25f};

            static const glm::vec2& s_MaximizeUV0 {0.333f, 0.0f};
            static const glm::vec2& s_MaximizeUV1 {0.5f, 0.25f};

            static const glm::vec2& s_RestoreUV0 {0.333f, 0.25f};
            static const glm::vec2& s_RestoreUV1 {0.5f, 0.5f};

            static const glm::vec2& s_CloseUV0 {0.5f, 0.25f};
            static const glm::vec2& s_CloseUV1 {0.666f, 0.5f};

            // Logo UVs
            static const glm::vec2& s_DX11UV0 {0.0f, 0.5f};
            static const glm::vec2& s_DX11UV1 {0.333f, 0.75f};

            static const glm::vec2& s_DX12UV0 {0.0f, 0.75f};
            static const glm::vec2& s_DX12UV1 {0.333f, 1.0f};

            static const glm::vec2& s_VulkanUV0 {0.333f, 0.5f};
            static const glm::vec2& s_VulkanUV1 {0.666f, 0.75f};

            static const glm::vec2& s_OpenGLESUV0 {0.333f, 0.75f};
            static const glm::vec2& s_OpenGLESUV1 {0.666f, 1.0f};

            static const glm::vec2& s_AMDUV0 {0.666f, 0.5f};
            static const glm::vec2& s_AMDUV1 {0.833f, 0.75f};

            static const glm::vec2& s_IntelUV0 {0.833f, 0.5f};
            static const glm::vec2& s_IntelUV1 {1.0f, 0.75f};

            static const glm::vec2& s_NvidiaUV0 {0.666f, 0.75f};
            static const glm::vec2& s_NvidiaUV1 {0.833f, 1.0f};

            static const glm::vec2& s_ARMUV0 {0.833f, 0.75f};
            static const glm::vec2& s_ARMUV1 {1.0f, 1.0f};

            tsl::sparse_map<Sentinel::UInt32, std::pair<glm::vec2, glm::vec2>> s_APIUVMap = {
                {0, {s_DX11UV0, s_DX11UV1}},
                {1, {s_DX12UV0, s_DX12UV1}},
                {2, {s_VulkanUV0, s_VulkanUV1}},
                {3, {s_OpenGLESUV0, s_OpenGLESUV1}}};

            tsl::sparse_map<Sentinel::UInt32, std::pair<glm::vec2, glm::vec2>> s_HardwareUVMap = {
                {0x10DE, {s_NvidiaUV0, s_NvidiaUV1}},
                {0x1002, {s_AMDUV0, s_AMDUV1}},
                {0x8086, {s_IntelUV0, s_IntelUV1}}};

            // 13.0f is the size of the default font. Change to the font size you use.
            static const Sentinel::Float s_BaseFontSize = 16.0f;
        }  // namespace ImGuiBaseUtils

        ImGuiBase::ImGuiBase(Sentinel::ContextData* context, Window::ScribeWindow* window)
            : m_Context(context), m_Window(window) {
            m_TexMemAllocator.Allocate(2);
        }

        ImGuiBase::~ImGuiBase() {
            m_TexMemAllocator.DeleteAll();
            m_TexMemAllocator.Deallocate();
        }

        void ImGuiBase::OnAttach() {
            Sentinel::Texture2DDataImportSettings settings;

            m_TitlebarSpriteSheetTex = Sentinel::Texture2DAPI::CreateTexture2DData(
                m_TexMemAllocator,
                m_Context,
                settings,
                TitleBar_SpriteSheet192128Texels,
                TitleBar_SpriteSheetWidth192,
                TitleBar_SpriteSheetHeight128,
                TitleBar_SpriteSheetBPP4);

            m_ContentBrowserSpriteSheetTex = Sentinel::Texture2DAPI::CreateTexture2DData(
                m_TexMemAllocator,
                m_Context,
                settings,
                ContentBrowser_SpriteSheet192128Texels,
                ContentBrowser_SpriteSheetWidth192,
                ContentBrowser_SpriteSheetHeight128,
                ContentBrowser_SpriteSheetBPP4);

            ImGuiIO& io = ImGui::GetIO();

            // Add Fonts
            // io.Fonts->AddFontFromFileTTF("Assets/Fonts/monasans/MonaSans-Bold.ttf", m_BaseFontSize);
            io.FontDefault = io.Fonts->AddFontFromFileTTF(
                "Assets/Fonts/monasans/MonaSans-Regular.ttf", ImGuiBaseUtils::s_BaseFontSize);

            // FontAwesome fonts need to have their sizes reduced by 2.0f/3.0f in order to align correctly
            Sentinel::Float iconFontSize = ImGuiBaseUtils::s_BaseFontSize * 2.45f / 3.0f;

            static const ImWchar icons_ranges_fa[] = {ICON_MIN_FA, ICON_MAX_16_FA, 0};
            // static const ImWchar icons_ranges_fab[] = {ICON_MIN_FAB, ICON_MAX_16_FAB, 0};
            static const ImWchar icons_ranges_mdi[] = {ICON_MIN_MDI, ICON_MAX_MDI, 0};
            ImFontConfig icons_config;
            icons_config.MergeMode = true;
            icons_config.PixelSnapH = true;
            icons_config.GlyphMinAdvanceX = iconFontSize;
            icons_config.OversampleH = 1;
            icons_config.OversampleV = 1;
            ImGui::GetIO().Fonts->Flags |= ImFontAtlasFlags_NoPowerOfTwoHeight;
            /*io.Fonts->AddFontFromFileTTF(
                "Assets/Fonts/fontawesome/fa-regular-400.ttf", iconFontSize, &icons_config, icons_ranges_fa);*/
            io.Fonts->AddFontFromFileTTF(
                "Assets/Fonts/fontawesome/fa-solid-900.ttf", iconFontSize, &icons_config, icons_ranges_fa);
            /* io.Fonts->AddFontFromFileTTF(
                 "Assets/Fonts/fontawesome/fa-brands-400.ttf", iconFontSize, &icons_config, icons_ranges_fab);*/

            io.Fonts->AddFontFromFileTTF(
                "Assets/Fonts/maticons/materialdesignicons-webfont.ttf",
                ImGuiBaseUtils::s_BaseFontSize,
                &icons_config,
                icons_ranges_mdi);

            m_SceneHierarchyPanel = new Panel::SceneHierarchyPanel();
            m_ContentBrowserPanel = new Panel::ContentBrowserPanel(m_ContentBrowserSpriteSheetTex);
        }

        void ImGuiBase::OnDetach() {
            delete m_SceneHierarchyPanel;
            delete m_ContentBrowserPanel;
        }

        void ImGuiBase::OnUpdate() {
        }

        void ImGuiBase::OnRender() {
        }

        void ImGuiBase::OnImGuiRender() {
            ImGui::PushStyleColor(ImGuiCol_ResizeGrip, 0);
            ImGui::PushStyleColor(ImGuiCol_ResizeGripActive, 0);
            ImGui::PushStyleColor(ImGuiCol_ResizeGripHovered, 0);

            RenderImGuiTitleBar();

            static bool dockspaceOpen = true;
            static bool opt_fullscreen_persistant = true;
            bool opt_fullscreen = opt_fullscreen_persistant;
            static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

            ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDocking;
            if (opt_fullscreen) {
                ImGuiViewport* viewport = ImGui::GetMainViewport();
                ImGui::SetNextWindowPos({viewport->WorkPos.x, viewport->WorkPos.y + ImGuiBaseUtils::s_TitleBarHeight});
                ImGui::SetNextWindowSize(
                    {viewport->WorkSize.x, viewport->WorkSize.y - ImGuiBaseUtils::s_TitleBarHeight});
                ImGui::SetNextWindowViewport(viewport->ID);
                ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
                ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
                window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize |
                                ImGuiWindowFlags_NoMove;
                window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
            }

            if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode) {
                window_flags |= ImGuiWindowFlags_NoBackground;
            }

            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
            ImGui::Begin("DockSpace", &dockspaceOpen, window_flags);
            ImGui::PopStyleVar();

            if (opt_fullscreen) ImGui::PopStyleVar(2);

            // DockSpace
            ImGuiIO& io = ImGui::GetIO();
            ImGuiStyle& style = ImGui::GetStyle();
            float minWindowSizeX = style.WindowMinSize.x;
            style.WindowMinSize.x = 370.0f;

            if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable) {
                ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
                ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
            }

            style.WindowMinSize.x = minWindowSizeX;

            // ImGui::SetNextWindowPos({viewport->WorkPos.x, viewport->WorkPos.y + 64});
            m_SceneHierarchyPanel->Display();
            m_ContentBrowserPanel->Display();

            ImGui::End();

            ImGui::PopStyleColor();
            ImGui::PopStyleColor();
            ImGui::PopStyleColor();
        }

        void ImGuiBase::OnPostRender() {
        }

        void ImGuiBase::RenderImGuiTitleBar() {
            ImGuiWindowFlags flags = ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoMove |
                                     ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize |
                                     ImGuiWindowFlags_NoBringToFrontOnFocus;

            // Display the Engine logo
            ImGuiViewport* viewport = ImGui::GetMainViewport();
            ImGui::SetNextWindowPos(viewport->WorkPos);
            ImGui::SetNextWindowSize({viewport->WorkSize.x, 64});
            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
            ImGui::Begin("Title_Bar", (bool*)0, flags);
            ImGui::PopStyleVar();
            ImGui::Image(
                (ImTextureID)Sentinel::Texture2DAPI::GetResource(m_TitlebarSpriteSheetTex),
                ImGuiBaseUtils::s_IconSize,
                ImGuiBaseUtils::s_IconUV0,
                ImGuiBaseUtils::s_IconUV1);

            //  Render the close button
            ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(7, 5));
            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 0.0f, 0.0f, 1.0f));
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.24f, 0.24f, 0.24f, 1.0f));

            ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.4f, 0.4f, 0.4f, 1.0f));
            ImGui::SetCursorPos({ImGui::GetIO().DisplaySize.x - 104, 0});

            if (ImGui::ImageButton(
                    "Minimize",
                    (ImTextureID)Sentinel::Texture2DAPI::GetResource(m_TitlebarSpriteSheetTex),
                    ImGuiBaseUtils::s_ControlBtnSize,
                    ImGuiBaseUtils::s_MinimizeUV0,
                    ImGuiBaseUtils::s_MinimizeUV1)) {
                m_Window->Minimize();
            }
            Sentinel::Bool minimizeHover = ImGui::IsItemHovered();

            ImGui::SetCursorPos({ImGui::GetIO().DisplaySize.x - 69, 0});
            if (ImGui::ImageButton(
                    "Maximize_Restore",
                    (ImTextureID)Sentinel::Texture2DAPI::GetResource(m_TitlebarSpriteSheetTex),
                    ImGuiBaseUtils::s_ControlBtnSize,
                    m_Window->IsMaximized() ? ImGuiBaseUtils::s_RestoreUV0 : ImGuiBaseUtils::s_MaximizeUV0,
                    m_Window->IsMaximized() ? ImGuiBaseUtils::s_RestoreUV1 : ImGuiBaseUtils::s_MaximizeUV1)) {
                m_Window->IsMaximized() ? m_Window->RestoreDown() : m_Window->Maximize();
            }
            Sentinel::Bool max_restoreHover = ImGui::IsItemHovered();
            ImGui::PopStyleColor();

            ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.91f, 0.1f, 0.15f, 1.0f));
            ImGui::SetCursorPos({ImGui::GetIO().DisplaySize.x - 34, 0});
            if (ImGui::ImageButton(
                    "Close",
                    (ImTextureID)Sentinel::Texture2DAPI::GetResource(m_TitlebarSpriteSheetTex),
                    ImGuiBaseUtils::s_ControlBtnSize,
                    ImGuiBaseUtils::s_CloseUV0,
                    ImGuiBaseUtils::s_CloseUV1)) {
                m_Window->InvokeShutdown();
            }
            Sentinel::Bool closeHover = ImGui::IsItemHovered();
            Window::BlockTitleBarDrag(minimizeHover || closeHover || max_restoreHover);
            ImGui::PopStyleColor();

            ImGui::PopStyleColor(2);
            ImGui::PopStyleVar();

            const Sentinel::ContextInfo& info = Sentinel::ContextAPI::GetContextInfo(m_Context);

            ImGui::SetCursorPos({ImGui::GetIO().DisplaySize.x - 104, 32.0f});
            ImGui::Image(
                (ImTextureID)Sentinel::Texture2DAPI::GetResource(m_TitlebarSpriteSheetTex),
                {32.0f, 32.0f},
                ImGuiBaseUtils::s_HardwareUVMap[(Sentinel::UInt32)info.VendorID].first,
                ImGuiBaseUtils::s_HardwareUVMap[(Sentinel::UInt32)info.VendorID].second);

            if (ImGui::IsItemHovered()) ImGui::SetTooltip("%s", info.Renderer);

            ImGui::SetCursorPos({ImGui::GetIO().DisplaySize.x - 69, 32.0f});
            ImGui::Image(
                (ImTextureID)Sentinel::Texture2DAPI::GetResource(m_TitlebarSpriteSheetTex),
                {64.0f, 32.0f},
                ImGuiBaseUtils::s_APIUVMap[(Sentinel::UInt32)info.APIID].first,
                ImGuiBaseUtils::s_APIUVMap[(Sentinel::UInt32)info.APIID].second);

            if (ImGui::IsItemHovered()) ImGui::SetTooltip("%s", info.Version);

            // RenderImGuiTitleBar_RenderInfoPanel();

            ImGui::End();
        }

        void ImGuiBase::RenderImGuiTitleBar_RenderInfoPanel() {
            ImGuiViewport* viewport = ImGui::GetMainViewport();
            ImGuiWindowFlags flags = ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoMove |
                                     ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize;

            ImGuiStyle& style = ImGui::GetStyle();

            // Perform calculations
            const Sentinel::Float titleBarHeight = ImGui::GetWindowSize().y;
            const Sentinel::Float windowPadding =
                (titleBarHeight - (ImGuiBaseUtils::s_BaseFontSize * 3 + style.ItemSpacing.y * 2)) * 0.5f;
            const Sentinel::Float halfWindowPadding = windowPadding * 0.5f;

            const Sentinel::ContextInfo& info = Sentinel::ContextAPI::GetContextInfo(m_Context);

            const Sentinel::Float rendererIconWidth = ImGui::CalcTextSize(ICON_FA_MICROCHIP).x;
            const Sentinel::Float apiIconWidth = ImGui::CalcTextSize(ICON_FA_NETWORK_WIRED).x;
            const Sentinel::Float versionIconWidth = ImGui::CalcTextSize(ICON_FA_INFO).x;

            const Sentinel::Float maxIconColumnWidth =
                ST_MAX(rendererIconWidth, ST_MAX(apiIconWidth, versionIconWidth));

            const Sentinel::Float specWidth =
                maxIconColumnWidth + ImGui::CalcTextSize(info.Renderer).x + windowPadding * 2 + style.CellPadding.x * 4;

            ImGui::SetNextWindowPos(
                {viewport->WorkPos.x + ImGui::GetIO().DisplaySize.x - specWidth - 104, viewport->Pos.y});

            // Render device info window

            ImGui::PushStyleColor(ImGuiCol_Border, {0.0f, 0.0f, 0.0f, 0.0f});
            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2 {halfWindowPadding, halfWindowPadding});
            ImGui::BeginChild("Spec", {specWidth, titleBarHeight}, true, flags);

            ImGui::BeginTable("SpecTable", 2, ImGuiTableFlags_SizingFixedFit);

            const Sentinel::Float cursorPosFactor = maxIconColumnWidth + halfWindowPadding + style.CellPadding.x;

            ImGui::TableNextRow();
            ImGui::TableSetColumnIndex(0);
            ImGui::SetCursorPosX(cursorPosFactor - rendererIconWidth);
            ImGui::Text(ICON_FA_MICROCHIP);
            ImGui::TableSetColumnIndex(1);
            ImGui::Text("%s", info.Renderer);

            ImGui::TableNextRow();
            ImGui::TableSetColumnIndex(0);
            ImGui::SetCursorPosX(cursorPosFactor - apiIconWidth);
            ImGui::Text(ICON_FA_NETWORK_WIRED);
            ImGui::TableSetColumnIndex(1);
            ImGui::Text("%s", info.API);

            ImGui::TableNextRow();
            ImGui::TableSetColumnIndex(0);
            ImGui::SetCursorPosX(cursorPosFactor - versionIconWidth);
            ImGui::Text(ICON_FA_INFO);
            ImGui::TableSetColumnIndex(1);
            ImGui::Text("%s", info.Version);

            ImGui::EndTable();

            ImGui::EndChild();
            ImGui::PopStyleVar();
            ImGui::PopStyleColor();
        }
    }  // namespace Rendering
}  // namespace Scribe