#include "ExampleLayer.h"

ExampleLayer::ExampleLayer()
	: Layer("ExampleLayer") {
	ST_TRACE("ExampleLayer constructor called");
}

void ExampleLayer::OnAttach() {}

void ExampleLayer::OnDetach() {}

void ExampleLayer::OnUpdate() {
	//ST_TRACE("ExampleLayer OnUpdate() running");
}

void ExampleLayer::OnEvent(Sentinel::Event& event) {}

void ExampleLayer::OnRender() {}

void ExampleLayer::OnImGuiRender() {}
