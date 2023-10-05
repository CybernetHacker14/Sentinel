#include "Panels/RenderInfoPanel.h"

#include <Sentinel/Graphics/Device/ContextAPI.h>

#include <imgui.h>
#include <imgui_internal.h>

namespace Scribe {
    namespace Panel {
        void RenderInfoPanel::DisplayInfoPanel() {
            if (m_Context == nullptr) return;

            ImGuiWindowFlags window_flags =
                ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoDecoration |
                ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings |
                ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoMouseInputs;

            ImGui::Begin("Statistics", (Sentinel::Bool*)0, window_flags);
            ImGuiViewport* viewport = ImGui::GetMainViewport();
            ImGui::SetWindowPos("Statistics", {viewport->WorkPos.x + ImGui::GetWindowWidth(), viewport->WorkPos.y});
            const Sentinel::ContextInfo& info = Sentinel::ContextAPI::GetContextInfo(m_Context);
            ImGui::Text("Device   : %s", info.Renderer);
            ImGui::Text("API      : %s", info.API);
            ImGui::Text("Version  : %s", info.Version);
            ImGui::End();
        }
    }  // namespace Panel
}  // namespace Scribe