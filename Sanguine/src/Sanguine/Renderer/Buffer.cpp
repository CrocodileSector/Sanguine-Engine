#include "sgpch.h"
#include "Buffer.h"

#include "RendererAPI.h"
#include "Sanguine/OpenGL/OpenGLBuffer.h"

namespace Sanguine
{
	Reference<VertexBuffer> VertexBuffer::Create(uint32_t size)
	{
		switch (RendererAPI::GetAPI())
		{
		case RendererAPI::API::None:		SG_CORE_ASSERT(false, "Renderer API was selected as None!"); return nullptr;
		case RendererAPI::API::OpenGL:		return CreateReference<OpenGLVertexBuffer>(size);
		}

		SG_CORE_ASSERT(false, "No Renderer API was selected!");
		return nullptr;
	}

	Reference<VertexBuffer> VertexBuffer::Create(float* vertices, uint32_t size)
	{
		switch (RendererAPI::GetAPI())
		{
		case RendererAPI::API::None:		SG_CORE_ASSERT(false, "Renderer API was selected as None!"); return nullptr;
		case RendererAPI::API::OpenGL:		return CreateReference<OpenGLVertexBuffer>(vertices, size);
		}

		SG_CORE_ASSERT(false, "No Renderer API was selected!");  
		return nullptr;
	}

	Reference<IndexBuffer> IndexBuffer::Create(uint32_t* indices, uint32_t count)
	{
		switch (RendererAPI::GetAPI())
		{
		case RendererAPI::API::None:		SG_CORE_ASSERT(false, "Renderer API was selected as None!"); return nullptr;
		case RendererAPI::API::OpenGL:		return CreateReference<OpenGLIndexBuffer>(indices, count);
		}

		SG_CORE_ASSERT(false, "No Renderer API was selected!");
		return nullptr;
	}
}