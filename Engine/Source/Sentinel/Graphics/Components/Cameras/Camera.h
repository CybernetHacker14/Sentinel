#pragma once

#include "Sentinel/Common/Common.h"
#include "Sentinel/Math/Math.h"
#include "Sentinel/Graphics/Components/RenderResources/Buffers/Constantbuffer.h"

#include <glm/glm.hpp>

namespace Sentinel
{
	enum class ProjectionMode {
		PERSPECTIVE = 0,
		ORTHOGRAPHIC = 1
	};

	class Camera : public ISharedRef {
	public:
		Camera();
		Camera(const Float width, const Float height);

		const ProjectionMode& GetProjectionMode() { return m_ProjectionMode; }
		void SetProjectionMode(const ProjectionMode& mode) { m_ProjectionMode = mode; }

		const glm::mat4& GetProjectionMatrix() const { return m_ProjectionMatrix; }
		const glm::mat4& GetViewMatrix() const { return m_ViewMatrix; }
		glm::mat4& GetViewProjectionMatrix() const { return SIMDMath::SSEMatrixMultiply(m_ProjectionMatrix, m_ViewMatrix); }

		const glm::vec3& GetCPosition() const { return m_Position; }
		const glm::vec3& GetCOrientation() const { return m_Orientation; }

		glm::vec3& GetPosition() { return m_Position; }
		glm::vec3& GetOrientation() { return m_Orientation; }

		void SetPosition(const glm::vec3& position) { m_Position = position; }
		void SetOrientation(const glm::vec3& orientation) { m_Orientation = orientation; }

		const Float& GetPositionX() const { return m_Position.x; }
		const Float& GetPositionY() const { return m_Position.y; }
		const Float& GetPositionZ() const { return m_Position.z; }

		void SetPositionX(const Float value) { m_Position.x = value; }
		void SetPositionY(const Float value) { m_Position.y = value; }
		void SetPositionZ(const Float value) { m_Position.z = value; }

		const Float& GetOrientationX() const { return m_Orientation.x; }
		const Float& GetOrientationY() const { return m_Orientation.y; }
		const Float& GetOrientationZ() const { return m_Orientation.z; }

		void SetOrientationX(const Float value) { m_Orientation.x = value; }
		void SetOrientationY(const Float value) { m_Orientation.y = value; }
		void SetOrientationZ(const Float value) { m_Orientation.z = value; }

		const glm::vec3& GetDirectionVectorFront() const { return m_DirectionFront; }
		const glm::vec3& GetDirectionVectorRight() const { return m_DirectionRight; }
		const glm::vec3& GetDirectionVectorUp() const { return m_DirectionUp; }

		void OnResize(const Float width, const Float height);
		void OnUpdate();

	public:
		inline static SharedRef<Camera> Create(const Float width, const Float height) {
			return CreateSharedRef<Camera>(width, height);
		}

	protected:
		void Init();

		void UpdateDirectionVectors();
		void UpdateProjectionMatrix();
		void UpdateViewMatrix();

	protected:
		inline static glm::vec3 m_WorldUp = { 0.0f, 1.0f, 0.0f };

	protected:
		ProjectionMode m_ProjectionMode = ProjectionMode::PERSPECTIVE;

		// Matrices
		glm::mat4 m_ProjectionMatrix = glm::mat4(1.0f);
		glm::mat4 m_ViewMatrix = glm::mat4(1.0f);

		// Transform
		glm::vec3 m_Position = { 0, 0, 0 };
		glm::vec3 m_Orientation = { 0.0f, 0.0f, 0.0f }; // x = Yaw, y = Pitch, z = Roll

		// Direction vectors
		glm::vec3 m_DirectionFront = { 0.0f, 0.0f, 0.0f };
		glm::vec3 m_DirectionUp = { 0.0f, 0.0f, 0.0f };
		glm::vec3 m_DirectionRight = { 0.0f, 0.0f, 0.0f };

		// Additional attributes
		Float m_PerspectiveFOV = 45.0f;
		Float m_PerspectiveNear = 0.01f, m_PerspectiveFar = 1000.0f;

		Float m_OrthographicSize = 10.0f;
		Float m_OrthographicNear = -100.0f, m_OrthographicFar = 100.0f;
		Float m_OrthographicRotation = 0.0f;

		Float m_AspectRatio = 1.778f;

		// Experimental
		Float m_Exposure = 0.0f;
		glm::vec3 m_FocalPoint = { 0.0f, 0.0f, 0.0f };

	protected:
		SharedRef<Constantbuffer> m_CameraConstantbuffer;
	};

}
