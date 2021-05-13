#pragma once

#include "Sanguine/Renderer/Texture.h"

#include <glad/glad.h>

namespace Sanguine
{
	class OpenGLTexture2D : public Texture2D
	{
	public:
		OpenGLTexture2D(uint32_t width, uint32_t height, void* data = nullptr);
		OpenGLTexture2D(const std::string& path);
		virtual ~OpenGLTexture2D();

		virtual uint32_t GetWidth() const override { return m_Width; }
		virtual uint32_t GetHeight() const override { return m_Height; }
		virtual std::string GetFilepath() const override { return m_Path; }
		virtual const uint32_t GetTextureID() const override { return m_TextureID; }

		virtual void SetData(void* data, uint32_t size) override;

		virtual void Bind() override;

		virtual bool operator==(const Texture& other) override { return m_RendererID == ((OpenGLTexture2D&)other).m_RendererID; }

	private:
		static int slot;

		std::string m_Path;
		uint32_t m_Width, m_Height;
		uint32_t m_TextureID;
		uint32_t m_RendererID;
		GLenum m_Format;
		GLenum m_InternalFormat;
	};
}