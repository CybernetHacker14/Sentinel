#include "ExampleLayer.h"

ExampleLayer::ExampleLayer() : Layer("ExampleLayer") {
    Sentinel::Application::Get().SubscribeToEvent(Sentinel::EventType::WindowResize, ST_BIND_EVENT_FN(OnWindowResize));

    m_AttachFunction = ST_BIND_EVENT_FN(ExampleLayer::OnAttach);
    m_DetachFunction = ST_BIND_EVENT_FN(ExampleLayer::OnDetach);
    m_UpdateFunction = ST_BIND_EVENT_FN(ExampleLayer::OnUpdate);
    m_RenderFunction = ST_BIND_EVENT_FN(ExampleLayer::OnRender);
    m_ImGuiRenderFunction = ST_BIND_EVENT_FN(ExampleLayer::OnImGuiRender);
}

void ExampleLayer::OnAttach() {
}

void ExampleLayer::OnDetach() {
}

void ExampleLayer::OnUpdate() {
}

void ExampleLayer::OnRender() {
}

void ExampleLayer::OnImGuiRender() {
}

void ExampleLayer::OnWindowResize(Sentinel::Event& event) {
}
