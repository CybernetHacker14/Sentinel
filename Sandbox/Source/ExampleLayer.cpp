#include "ExampleLayer.h"

ExampleLayer::ExampleLayer()
	: Layer("ExampleLayer") {
	Sentinel::Application::Get().SubscribeToEvent(Sentinel::EventType::WindowResize,
		ST_BIND_EVENT_FN(OnWindowResize));
}

void ExampleLayer::OnAttach() {}

void ExampleLayer::OnDetach() {}

void ExampleLayer::OnUpdate() {}

void ExampleLayer::OnRender() {}

void ExampleLayer::OnImGuiRender() {}

void ExampleLayer::OnWindowResize(Sentinel::Event& event) {}
