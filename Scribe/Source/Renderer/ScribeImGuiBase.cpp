#include "Renderer/ScribeImGuiBase.h"
#include "Window/ScribeWindow.h"

#include <Sentinel/Graphics/Texture/Texture2DAPI.h>

#include <imgui.h>

#include "Icons/Title/iconbw64.inl"
#include "Icons/Title/close_dark20.inl"

namespace Scribe {
    namespace Rendering {
        static Sentinel::Bool genericWindowOpen = true;

        ScribeImGuiBase::ScribeImGuiBase(Sentinel::ContextData* context, Window::ScribeWindow* window)
            : Sentinel::Layer("EditorImGuiBase"), m_Context(context), m_Window(window) {
            m_AttachFunction = ST_BIND_EVENT_FN(ScribeImGuiBase::OnAttach);
            m_ImGuiRenderFunction = ST_BIND_EVENT_FN(ScribeImGuiBase::OnImGuiRender);

            m_TexMemAllocator.AllocateMemoryBlock(10);
        }

        ScribeImGuiBase::~ScribeImGuiBase() {
            m_TexMemAllocator.DeleteAll();
            m_TexMemAllocator.DeallocateMemoryBlock();
        }

        void ScribeImGuiBase::OnAttach() {
            Sentinel::Texture2DDataImportSettings settings;
            m_LogoTex = Sentinel::Texture2DAPI::CreateTexture2DData(
                m_TexMemAllocator,
                m_Context,
                settings,
                &(iconBW64Pixels[0]),
                iconBW64Width,
                iconBW64Height,
                iconBW64BPP);

            m_CloseTex = Sentinel::Texture2DAPI::CreateTexture2DData(
                m_TexMemAllocator,
                m_Context,
                settings,
                &(close_dark64Pixels[0]),
                close_dark64Width,
                close_dark64Height,
                close_dark64BPP);

            ST_ENGINE_INFO("{0}", m_TexMemAllocator.GetTotalAllocations());
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
                ImGui::SetNextWindowPos(viewport->WorkPos);
                ImGui::SetNextWindowSize(viewport->WorkSize);
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

            ImGui::End();
        }

        void ScribeImGuiBase::OnPostRender() {
        }

        void ScribeImGuiBase::RenderImGuiTitleBar() {
            ImGuiWindowFlags flags = ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoMove |
                                     ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize;

            ImGuiStyle& style = ImGui::GetStyle();

            // Display the Engine logo
            ImGuiViewport* viewport = ImGui::GetMainViewport();
            ImGui::SetNextWindowPos(viewport->WorkPos);
            ImGui::SetNextWindowSize({viewport->WorkSize.x, 64});
            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
            ImGui::Begin("Title_Bar", &genericWindowOpen, flags);
            ImGui::PopStyleVar();
            ImGui::Image((ImTextureID)Sentinel::Texture2DAPI::GetResource(m_LogoTex), {64, 64});

            ImGui::SetCursorPos({ImGui::GetIO().DisplaySize.x - 34, 0});
            //  Render the close button
            ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(7, 5));
            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 0.0f, 0.0f, 1.0f));
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.24f, 0.24f, 0.24f, 1.0f));
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.91f, 0.1f, 0.15f, 1.0f));
            if (ImGui::ImageButton("Close", (ImTextureID)Sentinel::Texture2DAPI::GetResource(m_CloseTex), {20, 20})) {
                m_Window->SecondaryShutdown();
            }
            Window::BlockTitleBarDrag(ImGui::IsItemHovered());
            ImGui::PopStyleColor(3);
            ImGui::PopStyleVar();

            ImGui::End();
        }
    }  // namespace Rendering
}  // namespace Scribe