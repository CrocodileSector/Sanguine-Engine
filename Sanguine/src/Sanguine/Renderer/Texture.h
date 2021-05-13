#pragma once

#include "Sanguine/Core/Core.h"

namespace Sanguine
{
	class Texture
	{
	public:
		virtual ~Texture() = default;

		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;
		virtual const uint32_t GetTextureID() const = 0;
		virtual std::string GetFilepath() const = 0;

		virtual void SetData(void* data, uint32_t size) = 0;

		virtual void Bind() = 0;

		virtual bool operator==(const Texture& other) = 0;
	};

	class Texture2D : public Texture
	{
	public:
		static Reference<Texture2D> Create(uint32_t width, uint32_t height, void* data = nullptr);
		static Reference<Texture2D> Create(const std::string& path);
	};
}