#include "sgpch.h"

#include "Window.h"

#include "Sanguine/Events/Event.h"
#include "Sanguine/Events/KeyEvent.h"
#include "Sanguine/Events/AppEvent.h"
#include "Sanguine/Events/MouseEvent.h"

namespace Sanguine
{
	static bool s_GLFWInitialized = false;

	static void GLFWErrorCallback(int error, const char* description)
	{
		SG_CORE_ERROR("Caught GLFW error: {0} with description: {1}", error, description);
	}

	Window* Window::Create(const WindowProperties& properties)
	{
		return new Window(properties);
	}

	Window::Window(const WindowProperties& properties)
	{
		Init(properties);
	}

	Window::~Window()
	{
		Shutdown();
	}

	void Window::Init(const WindowProperties& properties)
	{
		m_WindowProperties.Title = properties.Title;
		m_WindowProperties.Width = properties.Width;
		m_WindowProperties.Height = properties.Height;

		if (!s_GLFWInitialized)
		{
			int succes = glfwInit();
			SG_CORE_ASSERT(succes, "Couldn't initialize GLFW!");

			glfwSetErrorCallback(GLFWErrorCallback);

			s_GLFWInitialized = true;
		}

		m_Window = glfwCreateWindow((int)m_WindowProperties.Width, (int)m_WindowProperties.Height, m_WindowProperties.Title.c_str(), nullptr, nullptr);

		m_Context = new OpenGLContext(m_Window);
		m_Context->Init();
		
		glfwSetWindowUserPointer(m_Window, &m_WindowProperties);
		SetVSync(true);

		glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int width, int height)
		{
			WindowProperties& data = *(WindowProperties*)glfwGetWindowUserPointer(window);
			data.Width = width;
			data.Height = height;

			WindowResizeEvent resizeEvent(width, height);
			data.EventCallback(resizeEvent);
		});

		glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window)
		{
			WindowProperties& data = *(WindowProperties*)glfwGetWindowUserPointer(window);

			WindowCloseEvent closeEvent;
			data.EventCallback(closeEvent);
		});

		glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
		{
			WindowProperties& data = *(WindowProperties*)glfwGetWindowUserPointer(window);

			Event* keyEvent = nullptr;

			switch (action)
			{
			case GLFW_PRESS:
				keyEvent = new KeyPressedEvent(key, 0);
				break;
			case GLFW_RELEASE:
				keyEvent = new KeyReleasedEvent(key);
				break;
			case GLFW_REPEAT:
				keyEvent = new KeyPressedEvent(key, 1);
				break;
			}

			data.EventCallback(*keyEvent);
		});

		glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window, int key, int action, int mods)
		{
			WindowProperties& data = *(WindowProperties*)glfwGetWindowUserPointer(window);

			Event* mouseEvent = nullptr;

			switch (action)
			{
			case GLFW_PRESS:
				mouseEvent = new MouseButtonPressedEvent(key);
				break;
			case GLFW_RELEASE:
				mouseEvent = new MouseButtonReleasedEvent(key);
				break;
			}

			data.EventCallback(*mouseEvent);
		});

		glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double xOffset, double yOffset)
		{
			WindowProperties& data = *(WindowProperties*)glfwGetWindowUserPointer(window);

			MouseScrolledEvent scrollEvent((float)xOffset, (float)yOffset);
			data.EventCallback(scrollEvent);
		});

		glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double xPos, double yPos)
		{
			WindowProperties& data = *(WindowProperties*)glfwGetWindowUserPointer(window);

			MouseMovedEvent moveEvent((float)xPos, (float)yPos);
			data.EventCallback(moveEvent);
		});

		glfwSetCharCallback(m_Window, [](GLFWwindow* window, unsigned int keycode)
		{
			WindowProperties& data = *(WindowProperties*)glfwGetWindowUserPointer(window);
			
			KeyTypedEvent keyTypedEvent(keycode);
			data.EventCallback(keyTypedEvent);
		});

		SG_CORE_INFO("Created window {0} ({1}, {2})", properties.Title, properties.Width, properties.Height);
	}

	void Window::Shutdown()
	{
		glfwDestroyWindow(m_Window);
	}

	void Window::OnUpdate()
	{
		glfwPollEvents();
		m_Context->SwapBuffers();
	}

	void Window::SetVSync(bool enabled)
	{
		if (enabled)
		{
			glfwSwapInterval(1);
		}
		else
		{
			glfwSwapInterval(0);
		}

		m_WindowProperties.VSync = enabled;
	}

	bool Window::IsVSync() const
	{
		return m_WindowProperties.VSync;
	}
}