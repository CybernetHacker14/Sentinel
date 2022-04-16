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

		ImGuiIO& io = ImGui::GetIO();
		ImVec2& data = ImGui::GetContentRegionAvail();

		ImGui::Begin("Test UI");
		ImGui::DragFloat3("Position", &m_Camera->GetPosition().x, 0.01f, -10.0f, 10.0f);
		ImGui::DragFloat3("Test UI", &m_Camera->GetOrientation().x, 0.01f, -10.0f, 10.0f);
		ImGui::Text("%f , %f", io.DisplaySize.x, io.DisplaySize.y);

		if (ImGui::Button("Projection Mode"))
		{
			m_Camera->SetProjectionMode(
				m_Camera->GetProjectionMode() == ProjectionMode::PERSPECTIVE ?
				ProjectionMode::ORTHOGRAPHIC :
				ProjectionMode::PERSPECTIVE
			);
		}
		ImGui::End();
	}
}
