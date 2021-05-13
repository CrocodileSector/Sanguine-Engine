#pragma once

namespace Sanguine
{
	class Texture2D;

	class TextureManager
	{
	public:
		static void Init();
		static Reference<Texture2D> LoadTexture(const std::string& Path, const char* fileName);
		static void SetTexture(const std::string name, Reference<Texture2D> texture);
		static Reference<Texture2D> GetTexture(const char* name);
		static Reference<Texture2D> GetTexture(uint32_t textureID);
		static uint32_t GetTextureID(const char* name);

		static std::unordered_map<std::string, Reference<Texture2D>> GetTextures();

	protected:
		TextureManager() = delete;
		~TextureManager() = delete;

	private:
		static std::unordered_map<std::string, Reference<Texture2D>> mapTextures;
		static std::vector<Reference<Texture2D>> vTextures;
	};
}