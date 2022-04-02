#include "stpch.h"
#include "Sentinel/GUI/ImGui/ImGuiDebugLayer.h"

#include <imgui.h>

namespace Sentinel
{
	ImGuiDebugLayer::ImGuiDebugLayer(SharedRef<Camera> camera) {
		m_Camera = camera;
	}

	void ImGuiDebugLayer::OnImGuiRender() {
		ImGui::Begin("Test Window");
		m_Rotation = m_Camera->GetOrientation();
		ImGui::SliderFloat3("Test UI", &m_Rotation.x, 0.0f, 360.0f);
		m_Camera->SetOrientation(m_Rotation);
		ImGui::End();
	}
}
