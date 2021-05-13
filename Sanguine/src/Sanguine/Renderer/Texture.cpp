#include "sgpch.h"
#include "Texture.h"

#include "RendererAPI.h"
#include "Sanguine/OpenGL/OpenGLTexture.h"

namespace Sanguine
{
	Reference<Texture2D> Texture2D::Create(uint32_t width, uint32_t height, void* data)
	{
		switch (RendererAPI::GetAPI())
		{
		case RendererAPI::API::None:		SG_CORE_ASSERT(false, "Renderer API was selected as None!"); return nullptr;
		case RendererAPI::API::OpenGL:		return std::make_shared<OpenGLTexture2D>(width, height, data);
		}

		SG_CORE_ASSERT(false, "No Renderer API was selected!");
		return nullptr;
	}

	Reference<Texture2D> Texture2D::Create(const std::string& path)
	{
		switch (RendererAPI::GetAPI())
		{
		case RendererAPI::API::None:		SG_CORE_ASSERT(false, "Renderer API was selected as None!"); return nullptr;
		case RendererAPI::API::OpenGL:		return std::make_shared<OpenGLTexture2D>(path);
		}

		SG_CORE_ASSERT(false, "No Renderer API was selected!");
		return nullptr;
	}
}