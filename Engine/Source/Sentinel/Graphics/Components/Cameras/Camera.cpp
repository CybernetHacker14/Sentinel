#include "stpch.h"
#include "Sentinel/Graphics/Components/Cameras/Camera.h"

#include <glm/gtx/norm.hpp>

namespace Sentinel
{
	Camera::Camera() {
		UpdateDirectionVectors();
		UpdateViewMatrix();
		UpdateProjectionMatrix();
	}

	Camera::Camera(const Float width, const Float height) {
		OnResize(width, height);
	}

	void Camera::OnResize(const Float width, const Float height) {
		m_AspectRatio = width / height;
		UpdateDirectionVectors();
		UpdateViewMatrix();
		UpdateProjectionMatrix();
	}

	void Camera::UpdateDirectionVectors() {

		m_DirectionFront = Math::FastNormalize(
			{
				Math::FastCos(glm::radians(GetYaw())) * Math::FastSin(glm::radians(GetPitch())),
				Math::FastSin(glm::radians(GetPitch())),
				Math::FastSin(glm::radians(GetYaw())) * Math::FastCos(glm::radians(GetPitch()))
			}
		);

		m_DirectionRight = Math::FastNormalize(SIMDMath::SSECrossProduct(m_DirectionFront, m_WorldUp));
		m_DirectionUp = Math::FastNormalize(SIMDMath::SSECrossProduct(m_DirectionRight, m_DirectionFront));
	}

	void Camera::UpdateProjectionMatrix() {

		switch (m_ProjectionMode)
		{
			case ProjectionMode::PERSPECTIVE:
			{
				m_ProjectionMatrix = glm::perspectiveRH(m_PerspectiveFOV, m_AspectRatio, m_PerspectiveNear, m_PerspectiveFar);
				break;
			}
			case ProjectionMode::ORTHOGRAPHIC:
			{
				Float left = -m_OrthographicSize * m_AspectRatio * 0.5f;
				Float right = m_OrthographicSize * m_AspectRatio * 0.5f;
				Float bottom = -m_OrthographicSize * 0.5f;
				Float top = m_OrthographicSize * 0.5f;

				m_ProjectionMatrix = glm::ortho(left, right, bottom, top, m_OrthographicNear, m_OrthographicFar);
			}
		}
	}

	void Camera::UpdateViewMatrix() {
		m_ViewMatrix = glm::lookAtRH(m_Position, m_Position + m_DirectionFront, m_DirectionUp);
	}
}
