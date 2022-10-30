#include "Renderer/ScribeImGuiBase.h"

#include <imgui.h>

namespace Scribe {
    namespace Rendering {
        ScribeImGuiBase::ScribeImGuiBase() : Sentinel::Layer("EditorImGuiBase") {
            m_ImGuiRenderFunction = ST_BIND_EVENT_FN(ScribeImGuiBase::OnImGuiRender);
        }

        ScribeImGuiBase::~ScribeImGuiBase() {
        }

        void ScribeImGuiBase::OnAttach() {
        }

        void ScribeImGuiBase::OnDetach() {
        }

        void ScribeImGuiBase::OnUpdate() {
        }

        void ScribeImGuiBase::OnRender() {
        }

        void ScribeImGuiBase::OnImGuiRender() {
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

            if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode) window_flags |= ImGuiWindowFlags_NoBackground;

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
    }  // namespace Rendering
}  // namespace Scribe