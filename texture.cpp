#include <fstream>
#include "global.h"
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

Texture::~Texture()
{
	for (SDL_Texture *texture : cache.at(name)) SDL_DestroyTexture(texture);
}

const std::vector<SDL_Texture*>& Texture::Fetch() const
{
	return cache.at(name);
}

std::unordered_map<std::string,std::vector<SDL_Texture*>> Texture::cache;
Json::Value Texture::filenames;

namespace Animation
{
	Adapter::Adapter::Adapter(const Frames& frames, const Animation::Interval& interval) : frames(frames), interval(interval)
	{
		current=frames.cbegin();
	}

	SDL_Texture* LoopAdapter::operator()()
	{
		Interval elapsed=std::chrono::steady_clock::now()-lastUpdate;
		if (elapsed > interval)
		{
			if (++current == frames.cend()) current=frames.cbegin();
			lastUpdate=std::chrono::steady_clock::now();
		}

		return *current;
	}

	PingPongAdapter::PingPongAdapter(const Frames &frames,const Interval &interval) : Adapter(frames,interval)
	{
		currentReverse=frames.crbegin();
	}

	SDL_Texture* PingPongAdapter::operator()()
	{
		Interval elapsed=std::chrono::steady_clock::now()-lastUpdate;
		Frames::const_iterator nextForward=std::next(current);
		Frames::const_reverse_iterator nextReverse=std::next(currentReverse);
		if (elapsed > interval)
		{
			if (nextForward == frames.cend())
			{
				if (nextReverse == frames.crend())
				{
					current=frames.cbegin();
					currentReverse=frames.crbegin();
				}
				else
				{
					currentReverse++;
				}
			}
			else
			{
				current++;
			}
			lastUpdate=std::chrono::steady_clock::now();
		}

		return nextForward == frames.cend() ? *currentReverse : *current;
	}
}
