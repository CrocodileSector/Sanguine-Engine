#include "sgpch.h"
#include "Shader.h"

#include "RendererAPI.h"
#include "Sanguine/OpenGL/OpenGLShader.h"

namespace Sanguine
{
	Reference<Shader> Shader::Create(const std::string& filepath)
	{
		switch (RendererAPI::GetAPI())
		{
		case RendererAPI::API::None:		SG_CORE_ASSERT(false, "Renderer API was selected as None!"); return nullptr;
		case RendererAPI::API::OpenGL:		return CreateReference<OpenGLShader>(filepath);
		}

		SG_CORE_ASSERT(false, "No Renderer API was selected!");
		return nullptr;
	}

	Reference<Shader> Shader::Create(const std::string& name, const std::string& vertexSource, const std::string& fragmentSource)
	{
		switch (RendererAPI::GetAPI())
		{
		case RendererAPI::API::None:		SG_CORE_ASSERT(false, "Renderer API was selected as None!"); return nullptr;
		case RendererAPI::API::OpenGL:		return CreateReference<OpenGLShader>(name, vertexSource, fragmentSource);
		}

		SG_CORE_ASSERT(false, "No Renderer API was selected!");
		return nullptr;
	}

	void ShaderLibrary::Add(const std::string& name, const Reference<Shader>& shader)
	{
		SG_CORE_ASSERT(Exists(name) == false, "Shader already exists!");
		m_Shaders[name] = shader;
	}

	void ShaderLibrary::Add(const Reference<Shader>& shader)
	{
		auto& name = shader->GetName();
		SG_CORE_ASSERT(Exists(name) == false, "Shader already exists!");
		m_Shaders[name] = shader;
	}

	Reference<Shader> ShaderLibrary::Load(const std::string& filepath)
	{
		auto shader = Shader::Create(filepath);
		Add(shader);

		return shader;
	}

	Reference<Shader> ShaderLibrary::Load(const std::string& name, const std::string& filepath)
	{
		auto shader = Shader::Create(filepath);
		Add(name, shader);

		return shader;
	}

	Reference<Shader> ShaderLibrary::Get(const std::string& name)
	{
		SG_CORE_ASSERT(Exists(name), "Shader not found!");
		return m_Shaders[name];
	}
}