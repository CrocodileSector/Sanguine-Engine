#pragma once

#include "Camera.h"

#include "Sanguine/Core/Timestep.h"
#include "Sanguine/Events/MouseEvent.h"
#include "Sanguine/Events/AppEvent.h"

#include <glm/glm.hpp>

namespace Sanguine
{
	class PerspectiveCamera : public Camera
	{
		friend class PerspectiveCameraController;

	public:
		PerspectiveCamera(float FoVy, float width, float height, float zNear, float zFar);

	public:
		void Update(const glm::vec3& position, const glm::vec3& direction);
		void Resize(float width, float height);

		void SetProjection(float FoVy, float width, float height, float zNear, float zFar);
		const glm::mat4& GetViewProjectionMatrix() const { return m_ViewProjectionMatrix; }

	private:
		glm::mat4 m_ViewProjectionMatrix;

		float m_Width				{ 0.0f };
		float m_Height				{ 0.0f };

		float m_FoV					{ 0.0f };
		float m_zNear				{ 0.0f };
		float m_zFar				{ 0.0f };
	};
}