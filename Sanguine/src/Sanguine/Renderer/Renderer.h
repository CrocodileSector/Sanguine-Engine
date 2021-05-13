#pragma once

#include "Sanguine/Core/Core.h"
#include "Sanguine/Renderer/RendererCommand.h"

#include <glm/gtx/transform.hpp>
#include <glm/gtx/quaternion.hpp>

namespace Sanguine
{
	class Scene;
	class Camera;
	class Shader;
	class Texture2D;
	class OpenGLMesh;

	class SceneRenderer
	{
	public:
		static void Initialize();

		static void SetContext(Reference<Scene> scene);

		static Reference<OpenGLMesh> GetMesh(const std::string& name);
		static Reference<OpenGLMesh> ConstructMesh(const std::string& fileLocation, const std::string& name);

		static void OnUpdate(Camera& camera, float delta);

		static void OnWindowResize(uint32_t width, uint32_t height);
	private:
		static void RenderMesh(Reference<OpenGLMesh> mesh, glm::mat4 model = glm::mat4{ 1.0f });

		static Reference<OpenGLMesh> ConstructCube(const std::string& name, Reference<Texture2D> texture);
		static Reference<OpenGLMesh> ConstructPlane(const std::string& name, Reference<Texture2D> texture);
		static Reference<OpenGLMesh> ConstructSphere(const std::string& name, Reference<Texture2D> texture);
		static Reference<OpenGLMesh> ConstructTeapot(const std::string& name, Reference<Texture2D> texture);
	};
}