#pragma once

#include <Sanguine.h>

#include "UI/SceneHierarchyPanel.h"


class GameWorld : public Sanguine::Layer, Sanguine::net::client_interface<GameMsg>
{
public:
	GameWorld();
	virtual ~GameWorld();

	virtual void OnAttach() override;
	virtual void OnDetach() override;

	virtual void OnImGuiRender() override;
	virtual void OnUpdate(Sanguine::Timestep ts) override;
	virtual void OnEvent(Sanguine::Event& event) override;

private:
	bool OnKeyPressed(Sanguine::KeyPressedEvent& e);

	void RenderSettingsPanel();
	void RenderViewSurface(); 
	void RenderChatBox();

	void UpdateClient();
	void UpdateController(Sanguine::Timestep ts);

	void ConstructDefaultScene();

private:
	glm::vec3 m_BackgrounColor{ 0.75f, 0.1f, 0.0f };
	Sanguine::PerspectiveCameraController m_CameraController;

	glm::vec2 m_ViewportSize;
	glm::vec3 m_DefaultCameraPosition{ 1.0f, 1.0f, 1.0f };
	glm::vec3 m_DefaultCameraDirection{ 1.0f, 0.0f, 0.0f };

	Sanguine::Reference<Sanguine::Scene> m_ActiveScene;
	Sanguine::Reference<Sanguine::Framebuffer> m_Framebuffer;

	std::unordered_map<uint32_t, ClientDescription> m_Clients;
	uint32_t m_ClientID = 0;

	bool m_WaitingOnConnection = true;
	std::vector<std::pair<uint32_t, std::string>> m_MessageList;

	float m_AverageFrameTimeMilliseconds = 0.0f;
};