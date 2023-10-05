#include "Panels/ViewportPanel.h"
#include "Renderer/SceneRenderer.h"

#include <Sentinel/Graphics/Output/FramebufferAPI.h>
#include <Sentinel/Graphics/Texture/RenderTexture2DAPI.h>

#include <Sentinel/Logging/Log.h>

#include <imgui.h>
#include <imgui_internal.h>

#include <glm/glm.hpp>

namespace Scribe {
    namespace Panel {
        void ViewportPanel::DisplayViewport() {
            if (m_Renderer == nullptr) return;

            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2 {0, 0});
            ImGui::Begin("Viewport", (bool*)0, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoCollapse);
            ImVec2 size = ImGui::GetWindowSize();
            Sentinel::UInt16 width = m_Renderer->GetFinalRTWidth();
            Sentinel::UInt16 height = m_Renderer->GetFinalRTHeight();

            if (size.x != width || size.y != height) { m_Renderer->Resize(size.x, size.y); }

            ImGui::Image(
                (ImTextureID)Sentinel::RenderTexture2DAPI::GetNativeSRV(m_Renderer->GetFinalRT()), {size.x, size.y});
            ImGui::End();

            ImGui::PopStyleVar();
        }
    }  // namespace Panel
}  // namespace Scribe