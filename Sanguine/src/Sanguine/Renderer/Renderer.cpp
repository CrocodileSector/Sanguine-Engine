#include "sgpch.h"
#include "Renderer.h"

#include "Sanguine/ECS/Scene.h"
#include "Sanguine/ECS/Components.h"
#include "Sanguine/Renderer/Camera.h"
#include "Sanguine/Renderer/Shader.h"
#include "Sanguine/OpenGL/OpenGLMesh.h"
#include "Sanguine/Physics/PhysicsEngine.h"
#include "Sanguine/Renderer/TextureManager.h"

namespace Sanguine
{
	static std::unordered_map<std::string, Reference<OpenGLMesh>> MeshPool;
	static Reference<Scene> ActiveScene;
	static Reference<Shader> LightShader;

	static glm::vec3 LightPosition{ -25.0f, 25.0f, 20.0f };
	static glm::vec4 LightColor{ 1.0f };

	Reference<PhysicsEngine> PhysicsEngine::s_PhysicsEngine = nullptr;
	static std::thread* PhysicsThread = nullptr;

	int MeshComponent::CustomMeshCount = 0;

    void SceneRenderer::Initialize()
	{
		RendererCommand::Init();
		TextureManager::Init();

		MeshPool["cube"] = ConstructCube("cube", TextureManager::GetTexture("blue.jpg"));
		MeshPool["plane"] = ConstructPlane("plane", TextureManager::GetTexture("black.jpg"));
		MeshPool["sphere"] = ConstructSphere("sphere", TextureManager::GetTexture("particle.png"));
		MeshPool["teapot"] = ConstructTeapot("teapot", TextureManager::GetTexture("default.png"));

		LightShader = Shader::Create("assets/shaders/Lighting.glsl");
		LightShader->Bind();
	}

	void SceneRenderer::SetContext(Reference<Scene> scene)
	{
		ActiveScene = scene;
	}

	void SceneRenderer::OnWindowResize(uint32_t width, uint32_t height)
	{
		RendererCommand::SetViewport(0, 0, width, height);
	}

	Reference<OpenGLMesh> SceneRenderer::GetMesh(const std::string& name)
	{
		if (MeshPool[name])
			return MeshPool[name];

		return nullptr;
	}

	Reference<OpenGLMesh> SceneRenderer::ConstructMesh(const std::string& fileLocation, const std::string& name)
	{
		Reference<OpenGLMesh> mesh = CreateReference<OpenGLMesh>(name);
		mesh->InitFromData(fileLocation);

		mesh->GetMaterial()->Texture = TextureManager::GetTexture("default.png");

		return mesh;
	}

	Reference<OpenGLMesh> SceneRenderer::ConstructCube(const std::string& name, Reference<Texture2D> texture)
	{
		Reference<OpenGLMesh> cubeMesh = CreateReference<OpenGLMesh>(name);

		std::vector<glm::vec3> positions =
		{
			//  x      y      z   
			{ -1.0f, -1.0f,  1.0f },
			{  1.0f, -1.0f,  1.0f },
			{ -1.0f,  1.0f,  1.0f },
			{  1.0f,  1.0f,  1.0f },
			{ -1.0f, -1.0f, -1.0f },
			{  1.0f, -1.0f, -1.0f },
			{ -1.0f,  1.0f, -1.0f },
			{  1.0f,  1.0f, -1.0f }
		};

		std::vector<uint32_t> indices
		{
			0, 1, 2,		1, 3, 2,
			2, 3, 7,		2, 7, 6,
			1, 7, 3,		1, 5, 7,
			6, 7, 4,		7, 5, 4,
			0, 4, 1,		1, 4, 5,
			2, 6, 4,		0, 2, 4
		};

		cubeMesh->InitFromData(indices, positions, positions.size());

		VertexData vertexData = cubeMesh->GetVertexData();
				
		vertexData.Normals.push_back(glm::vec3(1.0f, 0.0f, 0.0f));
		vertexData.Normals.push_back(glm::vec3(-1.0f, 0.0f, 0.0f));
		vertexData.Normals.push_back(glm::vec3(0.0f, 1.0f, 0.0f));
		vertexData.Normals.push_back(glm::vec3(1.0f, 1.0f, 1.0f));
		vertexData.Normals.push_back(glm::vec3(0.0f, -1.0f, 0.0f));
		vertexData.Normals.push_back(glm::vec3(0.0f, 0.0f, 1.0f));
		vertexData.Normals.push_back(glm::vec3(0.0f, 0.0f, -1.0f));
		vertexData.Normals.push_back(glm::vec3(1.0f, 1.0f, 1.0f));
				
		vertexData.TexCoords.push_back(glm::vec2(positions[0].x, positions[0].y));
		vertexData.TexCoords.push_back(glm::vec2(positions[1].x, positions[1].y));
		vertexData.TexCoords.push_back(glm::vec2(positions[2].x, positions[2].y));
		vertexData.TexCoords.push_back(glm::vec2(positions[3].x, positions[3].y));
		vertexData.TexCoords.push_back(glm::vec2(positions[4].x, positions[4].y));
		vertexData.TexCoords.push_back(glm::vec2(positions[5].x, positions[5].y));
		vertexData.TexCoords.push_back(glm::vec2(positions[6].x, positions[6].y));
		vertexData.TexCoords.push_back(glm::vec2(positions[7].x, positions[7].y));
		
		cubeMesh->SetVertexData(vertexData);
		cubeMesh->GetMaterial()->Texture = texture;

		return cubeMesh;
	}

	Reference<OpenGLMesh> SceneRenderer::ConstructPlane(const std::string& name, Reference<Texture2D> texture)
	{
		Reference<OpenGLMesh> wallMesh = CreateReference<OpenGLMesh>(name);
		wallMesh->InitFromData("assets/models/Primitives/plane50.obj");

		wallMesh->GetMaterial()->Texture = texture;
		wallMesh->GetMaterial()->Color = glm::vec3(0.0f);

		return wallMesh;
	}

	Reference<OpenGLMesh> SceneRenderer::ConstructSphere(const std::string& name, Reference<Texture2D> texture)
	{
		Reference<OpenGLMesh> sphereMesh = CreateReference<OpenGLMesh>(name);
		sphereMesh->InitFromData("assets/models/Primitives/sphere.obj");

		sphereMesh->GetMaterial()->Texture = texture;

		return sphereMesh;
	}

	Reference<OpenGLMesh> SceneRenderer::ConstructTeapot(const std::string& name, Reference<Texture2D> texture)
	{
		Reference<OpenGLMesh> teapotMesh = CreateReference<OpenGLMesh>(name);
		teapotMesh->InitFromData("assets/models/Primitives/teapot.obj");

		teapotMesh->GetMaterial()->Texture = texture;

		return teapotMesh;
	}

	void SceneRenderer::OnUpdate(Camera& camera, float delta)
	{
		Sanguine::PhysicsEngine::Get()->Update(delta);

		auto view = ActiveScene->m_Registry.view<ColliderComponent, TagComponent, TransformComponent, MeshComponent>();
		for (auto entity : view)
		{
			auto [collider, tag, transform, mesh] = view.get<ColliderComponent, TagComponent, TransformComponent, MeshComponent>(entity);

			if (collider.IsRemoteView()) continue;
			if (tag.Tag == "floor" || tag.Tag == "wall0" || tag.Tag == "wall1" || tag.Tag == "wall2" || tag.Tag == "ceiling") continue;

			ColliderComponent otherCollider = PhysicsEngine::Get()->GetCollider(tag.Tag);
			collider = otherCollider;
			transform.Translation = collider.GetState().Position;
			

			mesh.GetMesh()->UpdateVertices(transform.Transform.Translate(transform.Translation));
		}

		LightShader->SetMat4("u_View", camera.GetViewMatrix());
		LightShader->SetMat4("u_Projection", camera.GetProjectionMatrix());
		LightShader->SetFloat3("u_LightColor", LightColor);
		LightShader->SetFloat3("u_LightPosition", LightPosition);

		auto group = ActiveScene->m_Registry.group<TransformComponent>(entt::get<MeshComponent, TagComponent>);
		for (auto entity : group)
		{
			auto [transform, mesh, tag] = group.get<TransformComponent, MeshComponent, TagComponent>(entity);

			if (mesh.GetMesh())
			{
				RenderMesh(mesh.GetMesh(), transform.Update());
			}
		}
	}

	void SceneRenderer::RenderMesh(Reference<OpenGLMesh> mesh, glm::mat4 model)
	{
		Reference<VertexArray> VAO = mesh->GetVertexArray();
		Reference<Material> material = mesh->GetMaterial();
		VertexData vertexData = mesh->GetVertexData();

		VAO->Bind();
		material->Texture->Bind();

		LightShader->SetMat4("u_Model", model);
		LightShader->SetFloat("u_AmbientK", material->Ambient);
		LightShader->SetFloat("u_SpecularK", material->Specular);
		LightShader->SetFloat3("u_ObjectColor", material->Color);
		LightShader->SetInt("u_Texture", material->Texture->GetTextureID());

		uint32_t verticesBufferSize = vertexData.VerticesCount * vertexData.BufferLayout.GetStride();
		float* vertices = new float[verticesBufferSize];

		Reference<VertexBuffer> vertexBuffer = VertexBuffer::Create(verticesBufferSize);
		vertexBuffer->SetLayout(vertexData.BufferLayout);
		VAO->SetVertexBuffer(vertexBuffer);

		uint32_t offset = 0;
		for (int i = 0; i < vertexData.VerticesCount; i++)
		{
			vertices[i + offset + 0] = vertexData.Vertices[i].x;
			vertices[i + offset + 1] = vertexData.Vertices[i].y;
			vertices[i + offset + 2] = vertexData.Vertices[i].z;

			vertices[i + offset + 3] = vertexData.Normals[i].x;
			vertices[i + offset + 4] = vertexData.Normals[i].y;
			vertices[i + offset + 5] = vertexData.Normals[i].z;

			vertices[i + offset + 6] = vertexData.TexCoords[i].x;
			vertices[i + offset + 7] = vertexData.TexCoords[i].y;

			offset += 7;
		}
		VAO->GetVertexBuffer()->SetData(vertices, verticesBufferSize);

		uint32_t count = VAO->GetIndexBuffer()->GetCount();

		if (mesh->GetMeshID() == "plane")
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			glLineWidth(4.0f);

			glDrawElements(mesh->GetDrawMode(), count, GL_UNSIGNED_INT, nullptr);

			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			glLineWidth(1.0f);
		}
		else
		{
			glDrawElements(mesh->GetDrawMode(), count, GL_UNSIGNED_INT, nullptr);
		}

		VAO->Unbind();
		vertexBuffer.reset();
		delete[] vertices;
	}
}