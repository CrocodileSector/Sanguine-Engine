#pragma once

#include "RendererAPI.h"

namespace Sanguine
{
	class RendererCommand
	{
	public:
		inline static void Init()
		{
			s_RendererAPI->Init();
		}

		inline static void Clear()
		{
			s_RendererAPI->Clear();
		}

		inline static void SetClearColor(const glm::vec4& color)
		{
			s_RendererAPI->SetClearColor(color);
		}

		inline static void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
		{
			s_RendererAPI->SetViewport(x, y, width, height);
		}

		inline static void DrawIndexed(const Reference<VertexArray>& vertexArray, uint32_t indexCount)
		{
			s_RendererAPI->DrawIndexed(vertexArray, indexCount);
		}

	private:
		static RendererAPI* s_RendererAPI;
	};
}