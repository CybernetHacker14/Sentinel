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
		ImGui::DragFloat3("Test UI", &m_Rotation.x, 0.01f, -10.0f, 10.0f);
		m_Camera->SetOrientation(m_Rotation);
		if (ImGui::Button("Projection Mode"))
		{
			m_Camera->SetProjectionMode(
				m_Camera->GetProjectionMode() == ProjectionMode::PERSPECTIVE ? ProjectionMode::ORTHOGRAPHIC :
				ProjectionMode::PERSPECTIVE
			);
		}
		ImGui::End();
	}
}
