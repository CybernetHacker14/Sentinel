#include "Panels/ViewportPanel.h"
#include "Renderer/SceneRenderer.h"

#include <Sentinel/Graphics/Texture/RenderTexture2DAPI.h>

#include <imgui.h>
#include <imgui_internal.h>

namespace Scribe {
    namespace Panel {
        void ViewportPanel::DisplayViewport() {
            if (m_Renderer == nullptr) return;

            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2 {0, 0});
            ImGui::Begin("Viewport", (bool*)0, ImGuiWindowFlags_NoScrollbar);
            ImVec2 size = ImGui::GetWindowSize();
            ImGui::Image(
                (ImTextureID)Sentinel::RenderTexture2DAPI::GetNativeSRV(m_Renderer->GetFinalRT()), {size.x, size.y});
            ImGui::End();
            ImGui::PopStyleVar();
        }
    }  // namespace Panel
}  // namespace Scribe