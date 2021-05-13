#include "sgpch.h"
#include "PerspectiveCameraController.h"

#include "Sanguine/Core/Input.h"
#include "Sanguine/Core/KeyCodes.h"
#include "Sanguine/Core/MouseButtonCodes.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/glm.hpp>


namespace Sanguine
{
	PerspectiveCameraController::PerspectiveCameraController(float FoVy, float width, float height, float zNear, float zFar)
		: m_Camera(FoVy, width, height, zNear, zFar)
	{

	}

	void PerspectiveCameraController::OnUpdate(Timestep delta)
	{
		m_Camera.Update(m_Position, m_Direction);
	}

	void PerspectiveCameraController::Move(const glm::vec3& offset)
	{
		m_Position += offset;
	}

	void PerspectiveCameraController::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);

		dispatcher.Dispatch<MouseMovedEvent>(SG_BIND_EVENT_FN(PerspectiveCameraController::OnMouseMovedEvent));
		dispatcher.Dispatch<WindowResizeEvent>(SG_BIND_EVENT_FN(PerspectiveCameraController::OnWindowResized));
	}

	void PerspectiveCameraController::RotateOY(float delta)
	{
		m_Direction = glm::normalize(glm::mat3(glm::rotate(glm::radians(delta), glm::vec3{ 0.0f, 1.0f, 0.0f })) * m_Direction);
	}


	void PerspectiveCameraController::RotateOX(float delta)
	{
		glm::vec3 localRight = glm::normalize(glm::cross(m_Direction, glm::vec3{ 0.0f, 1.0f, 0.0f }));
		glm::mat3 rotationMat = glm::rotate(glm::radians(delta), localRight);

		m_Direction = glm::normalize(rotationMat * m_Direction);
	}

	bool PerspectiveCameraController::OnMouseMovedEvent(MouseMovedEvent& e)
	{
		if (Input::IsMouseButtonPressed(SG_MOUSE_BUTTON_2))
		{
			float xoffset = e.GetX() - m_PrevXOffset;
			float yoffset = e.GetY() - m_PrevYOffset;

			RotateOY(-xoffset);
			RotateOX(-yoffset);

			m_PrevXOffset = e.GetX();
			m_PrevYOffset = e.GetY();
		}

		return false;
	}

	bool PerspectiveCameraController::OnWindowResized(WindowResizeEvent& e)
	{
		m_Camera.Resize((float)e.GetWidth(), (float)e.GetHeight());

		return false;
	}
}