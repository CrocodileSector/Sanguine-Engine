#pragma once

#include "Scene.h"
#include "Sanguine/Core/Core.h"

namespace Sanguine
{
	class SceneSerializer
	{
	public:
		SceneSerializer(const Reference<Scene>& scene);

		void Serialize(const std::string& filepath);
		bool Deserialize(const std::string& filepath);

	private:
		Reference<Scene> m_Scene;
	};
}