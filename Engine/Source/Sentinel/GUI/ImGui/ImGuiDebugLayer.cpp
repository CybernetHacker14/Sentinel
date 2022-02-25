#include "stpch.h"
#include "Sentinel/GUI/ImGui/ImGuiDebugLayer.h"

#include <imgui.h>

namespace Sentinel
{
	ImGuiDebugLayer::ImGuiDebugLayer(SharedRef<Camera> camera) {
		m_Camera = camera;
	}

	void ImGuiDebugLayer::OnImGuiRender() {
		ImGui::ShowDemoWindow();
	}
}
