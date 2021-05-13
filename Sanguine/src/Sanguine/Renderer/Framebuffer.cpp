#include "sgpch.h"
#include "Framebuffer.h"

#include "Sanguine/Renderer/RendererAPI.h"
#include "Sanguine/OpenGL/OpenGLFramebuffer.h"

namespace Sanguine
{

	Reference<Framebuffer> Framebuffer::Create(const FramebufferSpecification& spec)
	{
		switch (RendererAPI::GetAPI())
		{
		case RendererAPI::API::None:    SG_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
		case RendererAPI::API::OpenGL:  return CreateReference<OpenGLFramebuffer>(spec);
		}

		SG_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

}
