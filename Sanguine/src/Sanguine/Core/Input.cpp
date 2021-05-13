#include "sgpch.h"
#include "Input.h"

#include "GLFW/glfw3.h"
#include "Sanguine/Core/Application.h"

namespace Sanguine
{
	Input* Input::s_Instance = new WindowsInput;

	bool WindowsInput::IsKeyPressedImpl(int keycode)
	{
		auto& window = Application::Get().GetWindow();
		auto keyState = glfwGetKey(window.GetGLFWWindow(), keycode);

		return keyState == GLFW_PRESS || keyState == GLFW_REPEAT;
	}

	bool WindowsInput::IsMouseButtonPressedImpl(int button)
	{
		auto& window = Application::Get().GetWindow();
		auto btnState = glfwGetMouseButton(window.GetGLFWWindow(), button);

		return btnState == GLFW_PRESS;
	}

	std::pair<float, float> WindowsInput::GetMousePositionImpl()
	{
		double xpos, ypos;

		auto& window = Application::Get().GetWindow();
		glfwGetCursorPos(window.GetGLFWWindow(), &xpos, &ypos);

		return { (float)xpos, (float)ypos };
	}

	float WindowsInput::GetMouseXImpl()
	{
		return GetMousePositionImpl().first;
	}

	float WindowsInput::GetMouseYImpl()
	{
		return GetMousePositionImpl().second;
	}
}