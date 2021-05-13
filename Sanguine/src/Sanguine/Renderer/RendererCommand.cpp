#include "sgpch.h"
#include "RendererCommand.h"

#include "Sanguine/OpenGL/OpenGLRendererAPI.h"

namespace Sanguine
{
	RendererAPI* RendererCommand::s_RendererAPI = new OpenGLRendererAPI();
}