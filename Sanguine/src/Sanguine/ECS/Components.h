#pragma once

#include "Sanguine/Core/Core.h"

#include "Sanguine/OpenGL/OpenGLTransform.h"
#include "Sanguine/OpenGL/OpenGLMesh.h"
#include "Sanguine/Renderer/Renderer.h"

#include "SceneCamera.h"
#include "EntityBehaviour.h"

namespace Sanguine
{
	struct TagComponent
	{
		std::string Tag;

		TagComponent() = default;
		TagComponent(const TagComponent&) = default;
		TagComponent(const std::string& tag)
			: Tag(tag) {}
	};

	struct TransformComponent
	{
		glm::vec3 Translation = { 0.0f, 0.0f, 0.0f };
		glm::vec3 Rotation = { 0.0f, 0.0f, 0.0f };
		glm::vec3 Scale = { 1.0f, 1.0f, 1.0f };

		Transform3D Transform;
		bool IsRemoteView = false;

		TransformComponent() = default;
		TransformComponent(const TransformComponent&) = default;
		TransformComponent(const Transform3D& transform)
			: Transform(transform)
		{

		}

		glm::mat4 Update()
		{
			Transform.Translate(Translation);

			glm::quat rotation = glm::quat(glm::radians(Rotation));
			Transform.SetRotation(glm::toMat4(rotation));

			Transform.SetScale(Scale);

			return Transform.GetModel();
		}
	};

	struct MeshComponent
	{
		static int CustomMeshCount;

		glm::vec4 Color;
		std::string Filepath;
		MeshType Type = MeshType::Cube;

		MeshComponent() = default;
		MeshComponent(const MeshComponent&) = default;

		MeshComponent(MeshType type)
			: Mesh(nullptr), Type(type)
		{

		}
		MeshComponent(const Reference<OpenGLMesh>& mesh, MeshType type)
			: Mesh(mesh), Type(type)
		{

		}

		bool ReloadMesh()
		{
			bool result = true;
			switch (Type)
			{
				case MeshType::Cube:
				{
					Mesh = SceneRenderer::GetMesh("cube");
					break;
				}
				case MeshType::Sphere:
				{
					Mesh = SceneRenderer::GetMesh("sphere");
					break;
				}
				case MeshType::Plane:
				{
					Mesh = SceneRenderer::GetMesh("plane");
					break;
				}
				case MeshType::Teapot:
				{
					Mesh = SceneRenderer::GetMesh("teapot");
					break;
				}
				case MeshType::Custom:
				{
					Mesh = SceneRenderer::ConstructMesh(Filepath, "custom" + CustomMeshCount);
					break;
				}
				default:
				{
					result = false;
				}
			}

			return result;
		}

		Reference<OpenGLMesh> GetMesh() { return Mesh; }
		void SetMesh(Reference<OpenGLMesh> mesh) { Mesh = mesh; }

	private:
		Reference<OpenGLMesh> Mesh;
	};

	struct CameraComponent
	{
		SceneCamera Camera;
		bool Primary = true; // TODO: think about moving to Scene
		bool FixedAspectRatio = false;

		CameraComponent() = default;
		CameraComponent(const CameraComponent&) = default;
	};

	struct NativeScriptComponent
	{
		EntityBehaviour* Instance = nullptr;

		EntityBehaviour* (*InstantiateScript)();
		void (*DestroyScript)(NativeScriptComponent*);

		template<typename T>
		void Bind()
		{
			InstantiateScript = []() { return static_cast<EntityBehaviour*>(new T()); };
			DestroyScript = [](NativeScriptComponent* nsc) { delete nsc->Instance; nsc->Instance = nullptr; };
		}
	};
}