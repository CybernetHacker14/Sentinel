#include "stpch.h"
#include "Sentinel/Graphics/Camera/Camera.h"

#include <glm/gtx/norm.hpp>

namespace Sentinel {
    Camera::Camera() {
        Init();
    }

    Camera::Camera(const Float width, const Float height) {
        OnResize(width, height);
        Init();
    }

    void Camera::OnResize(const Float width, const Float height) {
        m_AspectRatio = width / height;
    }

    void Camera::OnUpdate() {
        UpdateRotation();
        UpdateProjectionMatrix();
        UpdateViewMatrix();
    }

    glm::mat4& Camera::GetViewProjection() {
        return m_Projection * m_View;
    }

    void Camera::Init() {
        m_Position = {0.0f, 0.0f, 5.0f};
        m_FOVRadians = 45.0f;
        m_Near = 0.1f;
        m_Far = 100.0f;
        m_Pitch = 0;
        m_Yaw = -Math::PI_HALF;
    }

    void Camera::UpdateRotation() {
        m_Pitch = std::max(std::min(m_Pitch, Math::PI_HALF), -Math::PI_HALF);

        glm::vec3 center = {
            (std::cos(glm::radians(-90 + m_Yaw)) * std::cos(glm::radians(m_Pitch))),  // X
            std::sin(glm::radians(m_Pitch)),                                          // Y
            (std::sin(glm::radians(-90 + m_Yaw)) * std::cos(glm::radians(m_Pitch)))   // Z
        };

        m_Front = glm::normalize(center);
    }

    void Camera::UpdateViewMatrix() {
        m_View = glm::lookAt(m_Position, m_Position + m_Front, m_Up);
    }

    void Camera::UpdateProjectionMatrix() {
        m_Projection = glm::perspective(m_FOVRadians, m_AspectRatio, m_Near, m_Far);
    }
}  // namespace Sentinel