#pragma once

#include <Sentinel.h>

class ExampleLayer : public Sentinel::Layer {
public:
	ExampleLayer();

	virtual ~ExampleLayer() = default;

	virtual void OnAttach() override;
	virtual void OnDetach() override;

	void OnUpdate() override;
	void OnEvent(Sentinel::Event& event) override;
	void OnRender() override;
	void OnImGuiRender() override;
private:
	uint32_t value = 0;
};
