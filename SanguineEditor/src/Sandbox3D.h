#pragma once

#include <Sanguine.h>

#include "UI/SceneHierarchyPanel.h"

class Sandbox3D : public Sanguine::Layer
{
public:
	Sandbox3D();
	virtual ~Sandbox3D();

	virtual void OnAttach() override;
	virtual void OnDetach() override;

	virtual void OnImGuiRender() override;
	virtual void OnUpdate(Sanguine::Timestep ts) override;
	virtual void OnEvent(Sanguine::Event& event) override;

	void NewScene();
	void OpenScene();
	void SaveSceneAs();

private:
	bool OnKeyPressed(Sanguine::KeyPressedEvent& e);

	void UpdateController(Sanguine::Timestep ts);

	void RenderSettingsPanel();
	void RenderViewSurface();

	void ConstructDefaultScene();

private:
	glm::vec3 m_BackgrounColor{ 0.75f, 0.1f, 0.0f };
	Sanguine::PerspectiveCameraController m_CameraController;

	glm::vec2 m_ViewportSize;
	glm::vec3 m_DefaultCameraPosition{ 1.0f, 1.0f, 1.0f };
	glm::vec3 m_DefaultCameraDirection{ 1.0f, 0.0f, 0.0f };

	int m_GizmoType = -1;
	Sanguine::SceneHierarchyPanel m_SceneHierarchy;
	Sanguine::Reference<Sanguine::Scene> m_ActiveScene;
	Sanguine::Reference<Sanguine::Framebuffer> m_Framebuffer;

	float m_AverageFrameTimeMilliseconds = 0.0f;
};