#pragma once

#include <Sentinel.h>

class ExampleLayer final : public Sentinel::Layer {
public:
	ExampleLayer();

	virtual ~ExampleLayer() = default;

	virtual void OnAttach() final;
	virtual void OnDetach() final;

	void OnUpdate() final;
	void OnRender() final;
	void OnImGuiRender() final;
private:
	void OnWindowResize(Sentinel::Event& event);

	uint32_t value = 0;
};
