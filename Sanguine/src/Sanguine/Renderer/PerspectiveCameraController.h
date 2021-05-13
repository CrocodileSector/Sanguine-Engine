#include "PerspectiveCamera.h"

#include "Sanguine/Core/Timestep.h"
#include "Sanguine/Events/MouseEvent.h"
#include "Sanguine/Events/AppEvent.h"
#include "Sanguine/Network/NetCommon.h"

#include <glm/glm.hpp>


namespace Sanguine
{
	class PerspectiveCameraController
	{
	public:
		PerspectiveCameraController(float FoVy, float width, float height, float zNear, float zFar);

		PerspectiveCamera& GetCamera() { return m_Camera; }
		const PerspectiveCamera& GetCamera() const { return m_Camera; }

		void OnUpdate(Timestep ts);
		void OnEvent(Event& e);

		void Move(const glm::vec3& offset);

		void SetPosition(const glm::vec3& position) { m_Position = position; }
		void SetDirection(const glm::vec3& direction) { m_Direction = direction; }

		glm::vec3& GetPosition() { return m_Position; }
		glm::vec3& GetDirection() { return m_Direction; }

	private:
		void RotateOY(float delta);
		void RotateOX(float delta);

		bool OnMouseMovedEvent(MouseMovedEvent& e);
		bool OnWindowResized(WindowResizeEvent& e);

	private:
		PerspectiveCamera m_Camera;

		float m_CameraTranslationSpeed{ 5.0f };
		float m_CameraRotationSpeed{ 45.0f };

		float m_PrevXOffset = 0.0f;
		float m_PrevYOffset = 0.0f;

		glm::vec3 m_Position { 1.f };
		glm::vec3 m_Direction{ 1.f };
	};
}