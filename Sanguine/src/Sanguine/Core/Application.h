#pragma once

#include "Core.h"
#include "Window.h"

#include "Sanguine/Core/LayerStack.h"
#include "Sanguine/Events/Event.h"
#include "Sanguine/Events/AppEvent.h"
#include "Sanguine/ImGui/ImGuiLayer.h"

namespace Sanguine
{
	class Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();
		void OnEvent(Event& e);

		void Close();

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);

		inline static Application& Get() { return *s_Instance; }

		inline Window& GetWindow()  { return *m_Window; }

	private:
		bool OnWindowClose(WindowCloseEvent& e);
		bool OnWindowResized(WindowResizeEvent& e);

		std::unique_ptr<Window> m_Window;
		bool m_Running = true;
		bool m_Minimized = false;

		LayerStack m_LayerStack;
		ImGuiLayer* m_ImGuiLayer;
		
		float m_LastFrameTime{ 0.0f };

		static Application* s_Instance;
	};

	Application* CreateApplication();
}