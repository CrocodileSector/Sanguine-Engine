#include "sgpch.h"
#include "OpenGLContext.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <gl/GL.h>

namespace Sanguine
{
	OpenGLContext::OpenGLContext(GLFWwindow* window)
		: m_WindowHandle(window)
	{
		SG_CORE_ASSERT(window, "Window hanlde is nullptr!");
	}

	void OpenGLContext::Init()
	{
		glfwMakeContextCurrent(m_WindowHandle);

		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		SG_CORE_ASSERT(status, "Failed to init GLad!");

		SG_CORE_INFO("OpenGL Info:");
		SG_CORE_INFO("	Manufactured: {0}", glGetString(GL_VENDOR));
		SG_CORE_INFO("	GPU Unit: {0}", glGetString(GL_RENDERER));
		SG_CORE_INFO("	Version: {0}", glGetString(GL_VERSION));
	}

	void OpenGLContext::SwapBuffers()
	{
		glfwSwapBuffers(m_WindowHandle);
	}
}