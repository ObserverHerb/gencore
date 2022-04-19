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

	const char *ERROR_LOADING="Failed to load texture";
	const char *ENCODING="UTF-8";
	if (!cache.contains(name))
	{
		if (filenames.get(name,ENCODING).isArray())
		{
			for (const Json::Value &path : filenames.get(name,ENCODING))
			{
				SDL_Texture *texture=IMG_LoadTexture(renderer,path.asString().c_str());
				if (!texture) throw std::runtime_error(ERROR_LOADING);
				cache[name].push_back(texture);
			}
		}
		else
		{
			SDL_Texture *texture=IMG_LoadTexture(renderer,filenames.get(name,ENCODING).asString().c_str());
			if (!texture) throw std::runtime_error(ERROR_LOADING); // TODO: display texture name in error message
			cache[name].push_back(texture);
		}
	}
	else
	{
		// TODO: when I have a logging facility, note that a texture by that name has already been loaded
	}
}

const std::vector<SDL_Texture*>& Texture::Fetch() const
{
	return cache.at(name);
}

std::unordered_map<std::string,std::vector<SDL_Texture*>> Texture::cache;
Json::Value Texture::filenames;
