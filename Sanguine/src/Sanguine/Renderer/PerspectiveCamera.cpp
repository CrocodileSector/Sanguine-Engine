#include "sgpch.h"
#include "PerspectiveCamera.h"

#include "Sanguine/Core/Input.h"
#include "Sanguine/Core/KeyCodes.h"
#include "Sanguine/Core/MouseButtonCodes.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/matrix.hpp>
#include <glm/glm.hpp>

namespace Sanguine 
{
	PerspectiveCamera::PerspectiveCamera(float FoVy, float width, float height, float zNear, float zFar)
		: Camera(glm::perspective(glm::radians(FoVy), width / height, zNear, zFar), glm::mat4(1.0f)),
		  m_Width(width), 
		  m_Height(height),
		  m_FoV(FoVy), 
		  m_zNear(zNear), 
		  m_zFar(zFar)
	{

	}

	void PerspectiveCamera::SetProjection(float FoVy, float width, float height, float zNear, float zFar)
	{
		m_ProjectionMatrix = glm::perspective(FoVy, width / height, zNear, zFar);
	}

	void PerspectiveCamera::Resize(float width, float height)
	{
		SetProjection(m_FoV, width, height, m_zNear, m_zFar);
	}

	void PerspectiveCamera::Update(const glm::vec3& position, const glm::vec3& direction)
	{
		m_ViewMatrix = glm::lookAt(position, position + direction, glm::vec3{ 0.0f, 1.0f, 0.0f });
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}
}