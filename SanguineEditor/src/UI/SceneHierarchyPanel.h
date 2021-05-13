#pragma once

#include "Sanguine/Core/Core.h"
#include "Sanguine/ECS/Scene.h"
#include "Sanguine/ECS/Entity.h"

namespace Sanguine
{
	class SceneHierarchyPanel
	{
	public:
		SceneHierarchyPanel() = default;
		SceneHierarchyPanel(const Reference<Scene>& scene);

		void SetContext(const Reference<Scene>& scene);

		void OnImGuiRender();

		Entity GetSelectedEntity() const { return m_SelectionContext; }

	private:
		void DrawEntityNode(Entity entity);
		void DrawComponents(Entity entity);

	private:
		Reference<Scene> m_Context;
		Entity m_SelectionContext;
	};
}