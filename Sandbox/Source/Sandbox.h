#pragma once

#include <Sentinel.h>
#include <Sentinel/GUI/ImGui/ImGuiLayer.h>

#include "RendererLayer.h"

namespace Sandbox {
    class Sandbox: public Sentinel::Application {
    public:
        Sandbox();

    private:
        void Run();

        void ProcessLayerUpdate();
        void ProcessLayerRender();
        void ProcessLayerImGuiRender();
        void ProcessLayerPostRender();

    private:
        Sentinel::ImGuiLayer* m_ImGuiLayer;
        Rendering::RendererLayer* m_RenderLayer;

    private:
        Sentinel::Bool m_Running = true;
        Sentinel::Bool m_Minimized = false;
    };
}  // namespace Sandbox

Sentinel::Application* Sentinel::CreateApplication() {
    return new Sandbox::Sandbox;
}