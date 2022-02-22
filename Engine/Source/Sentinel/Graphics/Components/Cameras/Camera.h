#pragma once

#include "Sentinel/Common/Common.h"
#include "Sentinel/Math/Math.h"

#include <glm/glm.hpp>

namespace Sentinel
{
	enum class ProjectionMode {
		PERSPECTIVE = 0,
		ORTHOGRAPHIC = 1
	};

	class Camera {
	public:
		Camera();
		Camera(const Float width, const Float height);

		const glm::mat4& GetProjectionMatrix() const { return m_ProjectionMatrix; }
		const glm::mat4& GetViewMatrix() const { return m_ViewMatrix; }
		glm::mat4& GetViewProjectionMatrix() const { return SIMDMath::SSEMatrixMultiply(m_ProjectionMatrix, m_ViewMatrix); }

		const glm::vec3& GetPosition() const { return m_Position; }
		const glm::vec3& GetOrientation() const { return m_Orientation; }

		const Float GetYaw() const { return m_Orientation.x; }
		const Float GetPitch() const { return m_Orientation.y; }
		const Float GetRoll() const { return m_Orientation.z; }

		void SetPosition(const glm::vec3& position) { m_Position = position; }
		void SetOrientation(const glm::vec3& orientation) { m_Orientation = orientation; }

		const Float SetYaw(Float yaw) { m_Orientation.x = yaw; }
		const Float SetPitch(Float pitch) { m_Orientation.y = pitch; }
		const Float SetRoll(Float roll) { m_Orientation.z = roll; }

		const glm::vec3& GetDirectionVectorFront() const { return m_DirectionFront; }
		const glm::vec3& GetDirectionVectorRight() const { return m_DirectionRight; }
		const glm::vec3& GetDirectionVectorUp() const { return m_DirectionUp; }

		void OnResize(const Float width, const Float height);
		void OnUpdate();

	protected:
		void UpdateDirectionVectors();
		void UpdateProjectionMatrix();
		void UpdateViewMatrix();

	protected:
		inline static glm::vec3 m_WorldUp = { 0.0f, 1.0f, 0.0f };

	protected:
		ProjectionMode m_ProjectionMode = ProjectionMode::PERSPECTIVE;

		glm::mat4 m_ProjectionMatrix = glm::mat4(1.0f);
		glm::mat4 m_ViewMatrix = glm::mat4(1.0f);

		glm::vec3 m_Position = { -5, 5, 5 };
		glm::vec3 m_Orientation = { 0.0f, 0.0f, 0.0f }; // x = Yaw, y = Pitch, z = Roll

		// Direction vectors
		glm::vec3 m_DirectionFront = { 0.0f, 0.0f, 0.0f };
		glm::vec3 m_DirectionUp = { 0.0f, 0.0f, 0.0f };
		glm::vec3 m_DirectionRight = { 0.0f, 0.0f, 0.0f };

		// Additional attributes
		Float m_PerspectiveFOV = 45.0f;
		Float m_PerspectiveNear = 0.01f, m_PerspectiveFar = 1000.0f;

		Float m_OrthographicSize = 10.0f;
		Float m_OrthographicNear = -2.0f, m_OrthographicFar = 2.0f;

		Float m_AspectRatio = 1.778f;

		// Experimental
		Float m_Exposure = 0.0f;
		glm::vec3 m_FocalPoint = { 0.0f, 0.0f, 0.0f };
	};

}
