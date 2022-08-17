#pragma once

#include <Sentinel.h>

class ExampleLayer final: public Sentinel::Layer {
public:
    ExampleLayer();
    ~ExampleLayer() = default;

protected:
    void OnAttach();
    void OnDetach();

    void OnUpdate();
    void OnRender();
    void OnImGuiRender();

private:
    void OnWindowResize(Sentinel::Event& event);

    uint32_t value = 0;
};
