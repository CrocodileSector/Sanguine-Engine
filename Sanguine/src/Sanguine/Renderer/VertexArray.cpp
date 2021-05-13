#include "sgpch.h"
#include "VertexArray.h"

#include "RendererAPI.h"
#include "Sanguine/OpenGL/OpenGLVertexArray.h"

namespace Sanguine
{
	Reference<VertexArray> VertexArray::Create()
	{
		switch (RendererAPI::GetAPI())
		{
		case RendererAPI::API::None:		SG_CORE_ASSERT(false, "Renderer API was selected as None!"); return nullptr;
		case RendererAPI::API::OpenGL:		return CreateReference<OpenGLVertexArray>();
		}

		SG_CORE_ASSERT(false, "No Renderer API was selected!");
		return nullptr;
	}
}