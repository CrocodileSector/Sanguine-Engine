#include "sgpch.h"
#include "Sandbox2D.h"

Sandbox2D::Sandbox2D()
	: Layer("2D Sandbox"), m_CameraController(1280.f / 720.f, true)
{
	
}

Sandbox2D::~Sandbox2D()
{

}

void Sandbox2D::OnAttach()
{
	m_CheckeredTexture = Sanguine::Texture2D::Create("assets/textures/Checkerboard.png");
}

void Sandbox2D::OnDetach()
{

}

void Sandbox2D::OnImGuiRender()
{
	ImGui::Begin("Settings");

	ImGui::ColorEdit3("Object Color", glm::value_ptr(m_ObjColor));

	ImGui::End();
}

void Sandbox2D::OnUpdate(Sanguine::Timestep ts)
{
	{
		m_CameraController.OnUpdate(ts);
	}

	{
		Sanguine::RendererCommand::SetClearColor(glm::vec4{ 0.25f, 0, 0, 1 });
		Sanguine::RendererCommand::Clear();
	}
	
	{
		Sanguine::Renderer2D::BeginScene(m_CameraController.GetCamera());

		Sanguine::Renderer2D::DrawQuad({ -1.0f, 0.0f }, { 0.8f, 0.8f }, { 0.1f, 0.2f, 0.8f, 1.f });
		Sanguine::Renderer2D::DrawQuad({  0.5f,-0.5f }, { 1.0f, 1.0f }, { 0.1f, 0.2f, 0.8f, 1.f });
		Sanguine::Renderer2D::DrawQuad({ -5.0f,-5.0f, -0.1f }, { 10.f, 10.f }, m_CheckeredTexture, 10.f, glm::vec4(1.f, .25f, .25f, 1.f));
		Sanguine::Renderer2D::DrawRotatedQuad({ -2.0f, 0.0f, 0.0f }, { 1.0f, 1.0f }, -45.f, { 0.8f, 0.2f, 0.3f, 1.f });

		Sanguine::Renderer2D::EndScene();
	}
}

void Sandbox2D::OnEvent(Sanguine::Event& event)
{
	m_CameraController.OnEvent(event);
}
