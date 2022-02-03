#include "stpch.h"
#include "Sentinel/Graphics/Components/Cameras/Camera.h"

namespace Sentinel
{
	void Camera::UpdateDirectionVectors() {
		Float sx, sy, sz, cx, cy, cz, theta;
	}

	void Camera::CalculateProjection() {

		switch (m_ProjectionMode)
		{
			case ProjectionMode::PERSPECTIVE:
			{
				m_ProjectionMatrix = glm::perspective(m_PerspectiveFOV, m_AspectRatio, m_PerspectiveNear, m_PerspectiveFar);
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
}
