#include "Panels/RenderInfoPanel.h"

#include <Sentinel/Graphics/Device/ContextAPI.h>

#include <imgui.h>
#include <imgui_internal.h>

namespace Scribe {
    namespace Panel {
        void RenderInfoPanel::DisplayInfoPanel() {
            ImGui::Begin("Statistics");
            const Sentinel::ContextInfo& info = Sentinel::ContextAPI::GetContextInfo(m_Context);
            ImGui::Text("Vendor   : %s", info.Vendor);
            ImGui::Text("Renderer : %s", info.Renderer);
            ImGui::Text("API      : %s", info.API);
            ImGui::Text("Version  : %s", info.Version);
            ImGui::End();
        }
    }  // namespace Panel
}  // namespace Scribe