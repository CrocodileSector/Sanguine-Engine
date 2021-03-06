#pragma once

#include "Texture.h"
#include "OrthographicCamera.h"

namespace Sanguine
{
	class Renderer2D
	{
	public:
		static void Init();
		static void Shutdown();

		static void BeginScene(const OrthographicCamera& camera);
		static void EndScene();

		static void Flush();

		//Primitves
		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color);
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color);
		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const Reference<Texture2D> texture, float tiles = 1.f, const glm::vec4& tint = glm::vec4(1.f));
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const Reference<Texture2D> texture, float tiles = 1.f, const glm::vec4& tint = glm::vec4(1.f));

		static void DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const glm::vec4& color);
		static void DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const glm::vec4& color);
		static void DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const Reference<Texture2D> texture, float tiles = 1.f, const glm::vec4& tint = glm::vec4(1.f));
		static void DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const Reference<Texture2D> texture, float tiles = 1.f, const glm::vec4& tint = glm::vec4(1.f));

	private:
		static void SetupNewBatch();
	};
}