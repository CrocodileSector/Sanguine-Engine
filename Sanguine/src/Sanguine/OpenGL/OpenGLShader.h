#pragma once

#include "Sanguine/Renderer/Shader.h"

#include <unordered_map>
#include <glad/glad.h>
#include <glm/glm.hpp>

namespace Sanguine
{
	class OpenGLShader : public Shader
	{
	public:
		OpenGLShader(const std::string& name, const std::string& vertexSource, const std::string& fragmentSource);
		OpenGLShader(const std::string& filepath);
		virtual ~OpenGLShader();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual const std::string& GetName() const override { return m_Name; }

		virtual void SetIntArray(const std::string& name, int* values, uint32_t count) override;
		void UploadUniformIntArray(const std::string& name, int* values, uint32_t count);

		virtual void SetInt(const std::string& name, int value) override;
		void UploadUniformInt(const std::string& name, int value);

		virtual void SetMat3(const std::string& name, const glm::mat3& matrix) override;
		void UploadUniformMat3(const std::string& name, const glm::mat3& matrix);
		virtual void SetMat4(const std::string& name, const glm::mat4& matrix) override;
		void UploadUniformMat4(const std::string& name, const glm::mat4& matrix);

		virtual void SetFloat(const std::string& name, float value) override;
		void UploadUniformFloat(const std::string& name, float value);
		virtual void SetFloat2(const std::string& name, const glm::vec2& values) override;
		void UploadUniformFloat2(const std::string& name, const glm::vec2& values);
		virtual void SetFloat3(const std::string& name, const glm::vec3& values) override;
		void UploadUniformFloat3(const std::string& name, const glm::vec3& values);
		virtual void SetFloat4(const std::string& name, const glm::vec4& values) override;
		void UploadUniformFloat4(const std::string& name, const glm::vec4& values);

	private:
		std::string ReadFile(const std::string& filepath);
		std::unordered_map<GLenum, std::string> Preprocess(const std::string& source);
		void Compile(std::unordered_map<GLenum, std::string> shaderSources);

	private:
		std::string m_Name;
		unsigned int m_RendererID;
	};
}