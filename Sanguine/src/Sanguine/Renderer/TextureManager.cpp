#include "sgpch.h"
#include "TextureManager.h"

#include "Sanguine/Renderer/Texture.h"

#define INVALID_TEXTURE_ID 999

namespace Sanguine
{
	std::unordered_map<std::string, Reference<Texture2D>> TextureManager::mapTextures;
	std::vector<Reference<Texture2D>> TextureManager::vTextures;

	void TextureManager::Init()
	{
		LoadTexture(RESOURCE_PATH::TEXTURES, "default.png");
		LoadTexture(RESOURCE_PATH::TEXTURES, "white.png");
		LoadTexture(RESOURCE_PATH::TEXTURES, "black.jpg");
		LoadTexture(RESOURCE_PATH::TEXTURES, "blue.jpg");
		LoadTexture(RESOURCE_PATH::TEXTURES, "noise.png");
		LoadTexture(RESOURCE_PATH::TEXTURES, "random.jpg");
		LoadTexture(RESOURCE_PATH::TEXTURES, "particle.png");
	}

	std::unordered_map<std::string, Reference<Texture2D>> TextureManager::GetTextures()
	{
		return mapTextures;
	}

	Reference<Texture2D> TextureManager::LoadTexture(const std::string& path, const char* fileName)
	{
		Reference<Texture2D> texture = GetTexture(fileName);

		if (texture)
		{
			return mapTextures[fileName];
		}

		texture = Texture2D::Create((path + '/' + fileName).c_str());

		if (texture == nullptr) 
		{
			texture.reset();
			return vTextures[0];
		}

		vTextures.push_back(texture);
		mapTextures[fileName] = texture;
		return texture;
	}

	void TextureManager::SetTexture(std::string name, Reference<Texture2D> texture)
	{
		mapTextures[name] = texture;
	}

	Reference<Texture2D> TextureManager::GetTexture(const char* name)
	{
		if (mapTextures[name])
			return mapTextures[name];

		return NULL;
	}

	Reference<Texture2D> TextureManager::GetTexture(uint32_t textureID)
	{
		if (textureID < vTextures.size())
			return vTextures[textureID];

		return NULL;
	}

	uint32_t TextureManager::GetTextureID(const char* name)
	{
		if (mapTextures[name])
			return mapTextures[name]->GetTextureID();

		return INVALID_TEXTURE_ID;
	}
}