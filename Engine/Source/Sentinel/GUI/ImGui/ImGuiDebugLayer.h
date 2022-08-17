#pragma once

#include "Sentinel/Layers/Layer.h"
#include "Sentinel/Graphics/Camera/Camera.h"

namespace Sentinel {
    class ImGuiDebugLayer final: public Layer {
    public:
        ImGuiDebugLayer(SharedRef<Camera> camera);
        ~ImGuiDebugLayer() = default;

        void OnImGuiRender();

    private:
        SharedRef<Camera> m_Camera;
        glm::vec3 m_Rotation;
        glm::vec3 m_Position;
    };
}  // namespace Sentinel
