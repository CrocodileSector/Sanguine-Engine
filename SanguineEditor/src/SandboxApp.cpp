#include "sgpch.h"

#include "Sandbox2D.h"
#include "Sandbox3D.h"

#include <Sanguine/Core/EntryPoint.h>

Sanguine::RendererAPI::API Sanguine::RendererAPI::s_API = Sanguine::RendererAPI::API::OpenGL;

class Sandbox : public Sanguine::Application
{
public:
	Sandbox() 
	{
		PushLayer(new Sandbox3D());
	}

	~Sandbox()
	{

	}
};

Sanguine::Application* Sanguine::CreateApplication()
{
	return new Sandbox();
}