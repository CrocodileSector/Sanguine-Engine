#pragma once

#include <Sanguine.h>

class Sandbox2D : public Sanguine::Layer
{
public:
	Sandbox2D();
	virtual ~Sandbox2D();

	virtual void OnAttach() override;
	virtual void OnDetach() override;

	virtual void OnImGuiRender() override;
	virtual void OnUpdate(Sanguine::Timestep ts) override;
	virtual void OnEvent(Sanguine::Event& event) override;

private:
	Sanguine::OrthographicCameraController m_CameraController;
	Sanguine::Reference<Sanguine::Texture2D> m_CheckeredTexture;
	glm::vec3 m_ObjColor{ 1.0f, 0.1f, 0.0f };
};