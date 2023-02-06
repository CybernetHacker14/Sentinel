#include "Renderer/ScribeImGuiBase.h"
#include "Window/ScribeWindow.h"
#include "Panels/SceneHierarchyPanel.h"

#include <Sentinel/Graphics/Texture/Texture2DAPI.h>

#include <imgui.h>

#include "Icons/Title/iconbw64.inl"
#include "Icons/Title/close_dark20.inl"
#include "Icons/Title/minimize_dark20.inl"
#include "Icons/Title/maximize_dark20.inl"
#include "Icons/Title/restore_down_dark20.inl"

namespace Scribe {
    namespace Rendering {
        static Sentinel::Bool genericWindowOpen = false;

        ScribeImGuiBase::ScribeImGuiBase(Sentinel::ContextData* context, Window::ScribeWindow* window)
            : m_Context(context), m_Window(window) {
            m_TexMemAllocator.AllocateMemoryBlock(1);
            m_SceneHierarchyPanel = new Panel::SceneHierarchyPanel();
        }

        ScribeImGuiBase::~ScribeImGuiBase() {
            delete m_SceneHierarchyPanel;
            m_TexMemAllocator.DeleteAll();
            m_TexMemAllocator.DeallocateMemoryBlock();
        }

        void ScribeImGuiBase::OnAttach() {
            Sentinel::Texture2DDataImportSettings settings;
            settings.TextureFilepath = "Assets/Icons/Title/TitlebarSpritesheet.png";

            m_SpriteSheetTex = Sentinel::Texture2DAPI::CreateTexture2DData(m_TexMemAllocator, m_Context, settings);
        }

        void ScribeImGuiBase::OnDetach() {
        }

        void ScribeImGuiBase::OnUpdate() {
        }

        void ScribeImGuiBase::OnRender() {
        }

        void ScribeImGuiBase::OnImGuiRender() {
            RenderImGuiTitleBar();

            static bool dockspaceOpen = true;
            static bool opt_fullscreen_persistant = true;
            bool opt_fullscreen = opt_fullscreen_persistant;
            static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

            ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDocking;
            if (opt_fullscreen) {
                ImGuiViewport* viewport = ImGui::GetMainViewport();
                ImGui::SetNextWindowPos({viewport->WorkPos.x, viewport->WorkPos.y + m_TitleBarHeight});
                ImGui::SetNextWindowSize({viewport->WorkSize.x, viewport->WorkSize.y - m_TitleBarHeight});
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

            ImGuiViewport* viewport = ImGui::GetMainViewport();
            // ImGui::SetNextWindowPos({viewport->WorkPos.x, viewport->WorkPos.y + 64});
            ImGui::Begin("Test Window");
            ImGui::End();

            m_SceneHierarchyPanel->DisplayScenePanel();

            ImGui::End();
        }

        void ScribeImGuiBase::OnPostRender() {
        }

        void ScribeImGuiBase::RenderImGuiTitleBar() {
            ImGuiWindowFlags flags = ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoMove |
                                     ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize;

            // Display the Engine logo
            ImGuiViewport* viewport = ImGui::GetMainViewport();
            ImGui::SetNextWindowPos(viewport->WorkPos);
            ImGui::SetNextWindowSize({viewport->WorkSize.x, 64});
            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
            ImGui::Begin("Title_Bar", &genericWindowOpen, flags);
            ImGui::PopStyleVar();
            ImGui::Image(
                (ImTextureID)Sentinel::Texture2DAPI::GetResource(m_SpriteSheetTex), m_IconSize, m_IconUV0, m_IconUV1);

            //  Render the close button
            ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(7, 5));
            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 0.0f, 0.0f, 1.0f));
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.24f, 0.24f, 0.24f, 1.0f));

            ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.4f, 0.4f, 0.4f, 1.0f));
            ImGui::SetCursorPos({ImGui::GetIO().DisplaySize.x - 104, 0});

            if (ImGui::ImageButton(
                    "Minimize",
                    (ImTextureID)Sentinel::Texture2DAPI::GetResource(m_SpriteSheetTex),
                    m_ControlBtnSize,
                    m_MinimizeUV0,
                    m_MinimizeUV1)) {
                m_Window->Minimize();
            }
            Sentinel::Bool minimizeHover = ImGui::IsItemHovered();

            ImGui::SetCursorPos({ImGui::GetIO().DisplaySize.x - 69, 0});
            if (ImGui::ImageButton(
                    "Maximize_Restore",
                    (ImTextureID)Sentinel::Texture2DAPI::GetResource(m_SpriteSheetTex),
                    m_ControlBtnSize,
                    m_Window->IsMaximized() ? m_RestoreUV0 : m_MaximizeUV0,
                    m_Window->IsMaximized() ? m_RestoreUV1 : m_MaximizeUV1)) {
                m_Window->IsMaximized() ? m_Window->RestoreDown() : m_Window->Maximize();
            }
            Sentinel::Bool max_restoreHover = ImGui::IsItemHovered();
            ImGui::PopStyleColor();

            ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.91f, 0.1f, 0.15f, 1.0f));
            ImGui::SetCursorPos({ImGui::GetIO().DisplaySize.x - 34, 0});
            if (ImGui::ImageButton(
                    "Close",
                    (ImTextureID)Sentinel::Texture2DAPI::GetResource(m_SpriteSheetTex),
                    m_ControlBtnSize,
                    m_CloseUV0,
                    m_CloseUV1)) {
                m_Window->InvokeShutdown();
            }
            Sentinel::Bool closeHover = ImGui::IsItemHovered();
            Window::BlockTitleBarDrag(minimizeHover || closeHover || max_restoreHover);
            ImGui::PopStyleColor();

            ImGui::PopStyleColor(2);
            ImGui::PopStyleVar();

            ImGui::End();
        }
    }  // namespace Rendering
}  // namespace Scribe