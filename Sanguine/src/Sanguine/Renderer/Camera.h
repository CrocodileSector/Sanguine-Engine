#pragma once

#include <glm/glm.hpp>

namespace Sanguine
{

	class Camera
	{
	public:
		Camera() = default;
		Camera(const glm::mat4& projection, const glm::mat4& view)
			: m_ProjectionMatrix(projection), m_ViewMatrix(view)
		{

		}

		virtual ~Camera() = default;

		const glm::mat4& GetProjectionMatrix() const { return m_ProjectionMatrix; }
		const glm::mat4& GetViewMatrix() const { return m_ViewMatrix; }

		glm::vec3& GetPosition() { return m_Position; }

	protected:
		glm::mat4 m_ProjectionMatrix = glm::mat4(1.0f);
		glm::mat4 m_ViewMatrix = glm::mat4(1.0f);

		glm::vec3 m_Position = glm::vec3(1.0f);
	};

}