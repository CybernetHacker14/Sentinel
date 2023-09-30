#include "Panels/RenderInfoPanel.h"

#include <Sentinel/Graphics/Device/ContextAPI.h>

#include <imgui.h>
#include <imgui_internal.h>

namespace Scribe {
    namespace Panel {
        void RenderInfoPanel::DisplayInfoPanel() {
            if (m_Context == nullptr) return;

            ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_AlwaysAutoResize;
            ImGui::Begin("Statistics", (Sentinel::Bool*)0, window_flags);
            const Sentinel::ContextInfo& info = Sentinel::ContextAPI::GetContextInfo(m_Context);
            ImGui::Text("Vendor   : %s", info.Vendor);
            ImGui::Text("Renderer : %s", info.Renderer);
            ImGui::Text("API      : %s", info.API);
            ImGui::Text("Version  : %s", info.Version);
            ImGui::End();
        }
    }  // namespace Panel
}  // namespace Scribe