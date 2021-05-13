#pragma once

#include "Camera.h"

#include "Sanguine/Core/Timestep.h"
#include "Sanguine/Events/MouseEvent.h"
#include "Sanguine/Events/AppEvent.h"

#include <glm/glm.hpp>

namespace Sanguine
{ 
	class OrthographicCamera : public Camera
	{
	public:
		OrthographicCamera(float left, float right, float bottom, float top);

		void SetProjection(float left, float right, float bottom, float top);

		void SetPosition(const glm::vec3& position) { m_Position = position; RecalculateViewMatrix(); }

		float GetRotation() const { return m_Rotation; }
		void SetRotation(float rotation) { m_Rotation = rotation; RecalculateViewMatrix(); }

		const glm::mat4& GetViewProjectionMatrix() const { return m_ViewProjectionMatrix; }
		
	private:
		void RecalculateViewMatrix();

	private:
		glm::mat4 m_ViewProjectionMatrix;

		float m_Rotation{ 0.0f };
	};

	class OrthographicCameraController
	{
	public:
		OrthographicCameraController(float aspectRatio, bool rotation = false);

		OrthographicCamera& GetCamera() { return m_Camera; }
		const OrthographicCamera& GetCamera() const { return m_Camera; }

		void OnUpdate(Timestep ts);
		void OnEvent(Event& e);

	private:
		bool OnMouseScrolled(MouseScrolledEvent& e);
		bool OnWindowResized(WindowResizeEvent& e);

	private:
		float m_AspectRatio;
		float m_ZoomLevel = 1.f;
		OrthographicCamera m_Camera;

		glm::vec3 m_CameraPosition{ 0.f };
		float m_CameraRotation = 0.f;

		float m_CameraTranslationSpeed = 5.f;
		float m_CameraRotationSpeed = 45.f;

		bool m_UseRotation = false;
	};
}