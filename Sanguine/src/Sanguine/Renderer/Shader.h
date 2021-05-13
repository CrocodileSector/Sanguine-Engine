#pragma once

#include "Sanguine/Core/Core.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Sanguine
{
	class Shader
	{
	public:
		virtual ~Shader() = default;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual const std::string& GetName() const = 0;

		virtual void SetInt(const std::string& name, int value) = 0;
		virtual void SetIntArray(const std::string& name, int* values, uint32_t count) = 0;
			 
		virtual void SetMat3(const std::string& name, const glm::mat3& matrix) = 0;
		virtual void SetMat4(const std::string& name, const glm::mat4& matrix) = 0;
		 	 
		virtual void SetFloat(const std::string& name, float value) = 0;
		virtual void SetFloat2(const std::string& name, const glm::vec2& values) = 0;
		virtual void SetFloat3(const std::string& name, const glm::vec3& values) = 0;
		virtual void SetFloat4(const std::string& name, const glm::vec4& values) = 0;

		static Reference<Shader> Create(const std::string& filepath);
		static Reference<Shader> Create(const std::string& name, const std::string& vertexSource, const std::string& fragmentSource);
	};

	class ShaderLibrary
	{
	public:
		void Add(const Reference<Shader>& shader);
		void Add(const std::string& name, const Reference<Shader>& shader);
		Reference<Shader> Load(const std::string& filepath);
		Reference<Shader> Load(const std::string& name, const std::string& filepath);

		bool Exists(const std::string& name) const { return m_Shaders.find(name) != m_Shaders.end(); }

		Reference<Shader> Get(const std::string& name);
	private:
		std::unordered_map<std::string, Reference<Shader>> m_Shaders;
	};
}