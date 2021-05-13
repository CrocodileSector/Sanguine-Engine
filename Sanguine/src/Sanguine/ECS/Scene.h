#pragma once

#include "Sanguine/Core/Timestep.h"
#include "Sanguine/Renderer/PerspectiveCamera.h"

#include "entt.hpp"

struct ClientDescription;

namespace Sanguine
{
	class Entity;

	class Scene
	{
		friend class Entity;
		friend class SceneRenderer;
		friend class SceneSerializer;
		friend class SceneHierarchyPanel;

	public:
		Scene();
		~Scene();

		Entity CreateEntity(const std::string& name = std::string());
		void DestroyEntity(Entity entity);

		void OnUpdate(Timestep ts, PerspectiveCamera& camera);

		void UpdateClientEntity(const ClientDescription& desc);

		void OnViewportResize(uint32_t width, uint32_t height);

		Entity GetPrimaryCameraEntity();
	private:
		template<typename T>
		void OnComponentAdded(Entity entity, T& component);
	private:
		entt::registry m_Registry;
		uint32_t m_ViewportWidth = 0, m_ViewportHeight = 0;
	};

}