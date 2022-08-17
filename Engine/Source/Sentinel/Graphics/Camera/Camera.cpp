#include "stpch.h"
#include "Sentinel/Graphics/Camera/Camera.h"

#include "Sentinel/Graphics/Buffer/ConstantbufferAPI.h"

#include "Sentinel/Graphics/Common/GraphicsMemoryManager.h"

#include <glm/gtx/norm.hpp>

namespace Sentinel {
    Camera::Camera(SharedRef<GraphicsMemoryManager> memoryHandle, ContextData* context) { Init(memoryHandle, context); }

    Camera::Camera(
        SharedRef<GraphicsMemoryManager> memoryHandle, ContextData* context, const Float width, const Float height) {
        OnResize(width, height);
        Init(memoryHandle, context);
    }

    void Camera::OnResize(const Float width, const Float height) { m_AspectRatio = (!height) ? 0 : width / height; }

    void Camera::OnUpdate() {
        UpdateDirectionVectors();
        UpdateViewMatrix();
        UpdateProjectionMatrix();
        ConstantbufferAPI::SetDynamicData(m_CameraConstantbuffer, &(GetViewProjectionMatrix()));
    }

    void Camera::Init(SharedRef<GraphicsMemoryManager> memoryHandle, ContextData* context) {
        m_CameraConstantbuffer = ConstantbufferAPI::CreateConstantbufferData(
            memoryHandle, context, sizeof(glm::mat4), 0, CBufferUsageType::DYNAMIC);
        ConstantbufferAPI::VSBind(m_CameraConstantbuffer);
        OnUpdate();
    }

    void Camera::UpdateDirectionVectors() {
        m_DirectionFront = Math::FastNormalize(
            {Math::FastCos(glm::radians(-90 + m_Orientation.x)) * Math::FastSin(glm::radians(m_Orientation.y)),
             Math::FastSin(glm::radians(m_Orientation.y)),
             Math::FastSin(glm::radians(-90 + m_Orientation.x)) * Math::FastCos(glm::radians(m_Orientation.y))});

        m_DirectionRight = Math::FastNormalize(SIMDMath::SSECrossProduct(m_DirectionFront, m_WorldUp));
        m_DirectionUp = Math::FastNormalize(SIMDMath::SSECrossProduct(m_DirectionRight, m_DirectionFront));
    }

    void Camera::UpdateProjectionMatrix() {
        switch (m_ProjectionMode) {
            case ProjectionMode::PERSPECTIVE: {
                m_ProjectionMatrix =
                    glm::perspectiveRH(m_PerspectiveFOV, m_AspectRatio, m_PerspectiveNear, m_PerspectiveFar);
                break;
            }
            case ProjectionMode::ORTHOGRAPHIC: {
                Float left = -m_OrthographicSize * m_AspectRatio * 0.5f;
                Float right = m_OrthographicSize * m_AspectRatio * 0.5f;
                Float bottom = -m_OrthographicSize * 0.5f;
                Float top = m_OrthographicSize * 0.5f;

                m_ProjectionMatrix = glm::orthoRH(left, right, bottom, top, m_OrthographicNear, m_OrthographicFar);
                break;
            }
        }
    }

    void Camera::UpdateViewMatrix() {
        switch (m_ProjectionMode) {
            case ProjectionMode::PERSPECTIVE: {
                glm::mat4 transform(1.0f);
                transform = glm::lookAtRH(m_Position, m_Position + m_DirectionFront, m_DirectionUp);
                transform = glm::rotate(transform, m_Orientation.x, glm::vec3(1.0f, 0.0f, 0.0f));
                transform = glm::rotate(transform, m_Orientation.y, glm::vec3(0.0f, 1.0f, 0.0f));
                transform = glm::rotate(transform, m_Orientation.z, glm::vec3(0.0f, 0.0f, 1.0f));

                m_ViewMatrix = glm::inverse(transform);
                break;
            }
            case ProjectionMode::ORTHOGRAPHIC: {
                glm::mat4 transform(1.0f);
                transform = glm::translate(transform, m_Position);
                transform = glm::rotate(transform, m_Orientation.x, glm::vec3(1.0f, 0.0f, 0.0f));
                transform = glm::rotate(transform, m_Orientation.y, glm::vec3(0.0f, 1.0f, 0.0f));
                transform = glm::rotate(transform, m_Orientation.z, glm::vec3(0.0f, 0.0f, 1.0f));

                m_ViewMatrix = glm::inverse(transform);
                break;
            }
        }
    }
}  // namespace Sentinel
