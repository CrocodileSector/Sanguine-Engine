#include "sgpch.h"
#include "Scene.h"

#include "Entity.h"
#include "Components.h"

#include "Sanguine/Physics/Collider.h"
#include "Sanguine/Physics/PhysicsEngine.h"

#include "Sanguine/Renderer/Renderer.h"
#include "Sanguine/Network/NetCommon.h"

#include <glm/glm.hpp>

namespace Sanguine
{
	Scene::Scene()
	{
	}

	Scene::~Scene()
	{
	}

	Entity Scene::CreateEntity(const std::string& name)
	{
		Entity entity = { m_Registry.create(), this };
		entity.AddComponent<TransformComponent>();
		auto& tag = entity.AddComponent<TagComponent>();
		tag.Tag = name.empty() ? "Entity " : name;
		return entity;
	}

	void Scene::DestroyEntity(Entity entity)
	{
		m_Registry.destroy(entity);
	}

	void Scene::OnUpdate(Timestep ts, PerspectiveCamera& camera)
	{
		// Update scripts
		{
			m_Registry.view<NativeScriptComponent>().each([=](auto entity, auto& nsc)
				{
					if (!nsc.Instance)
					{
						nsc.Instance = nsc.InstantiateScript();
						nsc.Instance->m_Entity = Entity{ entity, this };
						nsc.Instance->OnCreate();
					}

					nsc.Instance->OnUpdate(ts);
				});
		}

		//Render meshes
		SceneRenderer::OnUpdate(camera, ts);
	}

	void Scene::OnViewportResize(uint32_t width, uint32_t height)
	{
		m_ViewportWidth = width;
		m_ViewportHeight = height;

		// Resize our non-FixedAspectRatio cameras
		auto view = m_Registry.view<CameraComponent>();
		for (auto entity : view)
		{
			auto& cameraComponent = view.get<CameraComponent>(entity);
			if (!cameraComponent.FixedAspectRatio)
				cameraComponent.Camera.SetViewportSize(width, height);
		}
	}

	void Scene::UpdateClientEntity(const ClientDescription& desc)
	{
		std::string entityTag = std::string("c_") + std::to_string(desc.PeerID);

		auto group = m_Registry.group<TagComponent>(entt::get<TransformComponent, MeshComponent>);
		for (auto entity : group)
		{
			auto [tag, transform, mesh] = group.get<TagComponent, TransformComponent, MeshComponent>(entity);

			if (tag.Tag == entityTag)
			{
				transform.Translation = desc.Position;
				transform.Rotation = desc.Direction;
				transform.Update();

				return;
			}
		}
	}

	Entity Scene::GetPrimaryCameraEntity()
	{
		auto view = m_Registry.view<CameraComponent>();
		for (auto entity : view)
		{
			const auto& camera = view.get<CameraComponent>(entity);
			if (camera.Primary)
				return Entity{ entity, this };
		}
		return {};
	}

	template<typename T>
	void Scene::OnComponentAdded(Entity entity, T& component)
	{
		static_assert(false);
	}

	template<>
	void Scene::OnComponentAdded<TransformComponent>(Entity entity, TransformComponent& component)
	{
	}

	template<>
	void Scene::OnComponentAdded<CameraComponent>(Entity entity, CameraComponent& component)
	{
		component.Camera.SetViewportSize(m_ViewportWidth, m_ViewportHeight);
	}

	template<>
	void Scene::OnComponentAdded<MeshComponent>(Entity entity, MeshComponent& component)
	{
		Reference<OpenGLMesh> mesh = component.GetMesh();
		if (component.Type != MeshType::Custom)
			if (mesh == nullptr) component.ReloadMesh();
	}

	template<>
	void Scene::OnComponentAdded<TagComponent>(Entity entity, TagComponent& component)
	{
	}

	template<>
	void Scene::OnComponentAdded<NativeScriptComponent>(Entity entity, NativeScriptComponent& component)
	{
	}

	template<>
	void Scene::OnComponentAdded<ColliderComponent>(Entity entity, ColliderComponent& component)
	{
		PhysicsEngine::Get()->Register(entity.GetComponent<TagComponent>().Tag, component);
	}
}