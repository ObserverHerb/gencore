#include <fstream>
#include "globals.h"
#include "texture.h"

Texture::Texture(const std::string &name) : name(name)
{
	if (filenames.isNull())
	{
		std::ifstream textureFilenamesFile(R"(textures.json)",std::ifstream::binary);
		textureFilenamesFile >> filenames;
	}

	if (!cache.contains(name))
	{
		SDL_Texture *texture=IMG_LoadTexture(renderer,filenames.get(name,"UTF-8").asString().c_str());
		if (!texture) throw std::runtime_error("Failed to load texture"); // FIXME: display texture name in error message
		cache.insert({name,texture});
	}
}

SDL_Texture* Texture::Fetch() const
{
	return cache.at(name);
}

std::unordered_map<std::string,SDL_Texture*> Texture::cache;
Json::Value Texture::filenames;