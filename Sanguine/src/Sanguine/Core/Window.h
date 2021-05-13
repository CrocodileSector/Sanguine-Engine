#pragma once

#include "sgpch.h"

#include "Sanguine/Core/Core.h"
#include "Sanguine/Events/Event.h"
#include "Sanguine/OpenGL/OpenGLContext.h"

struct GLFWwindow;

namespace Sanguine
{
	class Window
	{
	public:
		using EventCallback = std::function<void(Event&)>;

		struct WindowProperties
		{
			std::string Title;
			unsigned int Width;
			unsigned int Height;
			bool VSync = false;
			EventCallback EventCallback;

			WindowProperties(const std::string& title = "Sanguine Engine", unsigned int width = 1280, unsigned int height = 720)
				: Title(title), Width(width), Height(height)
			{

			}
		};

		Window(const WindowProperties& properties);
		virtual ~Window();

		void Init(const WindowProperties& properties);
		void OnUpdate();
		void Shutdown();

		inline GLFWwindow* GetGLFWWindow() { return m_Window; }
		inline void SetEventCallback(const EventCallback& callback) { m_WindowProperties.EventCallback = callback; }
		inline unsigned int GetWidth() const { return m_WindowProperties.Width; }
		inline unsigned int GetHeight() const { return m_WindowProperties.Height; };

		void SetVSync(bool enabled);
		bool IsVSync() const;

		static Window* Create(const WindowProperties& props = WindowProperties());

	private:
		GLFWwindow* m_Window;
		WindowProperties m_WindowProperties;
		GraphicsContext* m_Context;
	};
}