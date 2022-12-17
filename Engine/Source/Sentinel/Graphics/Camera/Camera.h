#pragma once

#include "Sentinel/Common/Common.h"
#include "Sentinel/Math/Math.h"

#define GLM_FORCE_RIGHT_HANDED
#include <glm/glm.hpp>

namespace Sentinel {
    enum class ProjectionMode : Bool { PERSPECTIVE = 0, ORTHOGRAPHIC = 1 };

    class Camera: public ISharedRef {
    public:
        Camera();
        Camera(const Float width, const Float height);

        void OnResize(const Float width, const Float height);
        void OnUpdate();

        glm::mat4& GetViewProjection();

    protected:
        void Init();

        void UpdateRotation();

        void UpdateViewMatrix();
        void UpdateProjectionMatrix();

    protected:
        inline static const glm::vec3 m_Up = {0.0f, 1.0f, 0.0f};

    protected:
        ProjectionMode m_ProjectionMode = ProjectionMode::PERSPECTIVE;

        Float m_Far = 0.01f;
        Float m_Near = 1000.0f;
        Float m_AspectRatio = 1.778f;
        Float m_FOVRadians;
        glm::vec3 m_Front;
        glm::vec3 m_Right;
        glm::vec3 m_Position;
        glm::mat4 m_Projection = glm::mat4(1.0f);
        glm::mat4 m_View = glm::mat4(1.0f);
        Float m_Pitch;
        Float m_Yaw;

        //// Matrices
        // glm::mat4 m_ProjectionMatrix = glm::mat4(1.0f);
        // glm::mat4 m_ViewMatrix = glm::mat4(1.0f);

        //// Transform
        // glm::vec3 m_Position = {0, 0, 0};
        // glm::vec3 m_Orientation = {0.0f, 0.0f, 0.0f};  // x = Yaw, y = Pitch, z = Roll

        //// Direction vectors
        // glm::vec3 m_DirectionFront = {0.0f, 0.0f, 0.0f};
        // glm::vec3 m_DirectionUp = {0.0f, 0.0f, 0.0f};
        // glm::vec3 m_DirectionRight = {0.0f, 0.0f, 0.0f};

        //// Additional attributes
        // Float m_PerspectiveFOV = 45.0f;
        // Float m_PerspectiveNear = 0.01f, m_PerspectiveFar = 1000.0f;

        // Float m_OrthographicSize = 10.0f;
        // Float m_OrthographicNear = -100.0f, m_OrthographicFar = 100.0f;
        // Float m_OrthographicRotation = 0.0f;

        // Float m_AspectRatio = 1.778f;

        //// Experimental
        // Float m_Exposure = 0.0f;
        // glm::vec3 m_FocalPoint = {0.0f, 0.0f, 0.0f};
    };

}  // namespace Sentinel