#pragma once

#include <SDL.h>
#include <json/json.h>
#include <unordered_map>
#include <string>


class Texture
{
public:
	Texture(const std::string &name);
	SDL_Texture* Fetch() const;
protected:
	static std::unordered_map<std::string,SDL_Texture*> cache;
	static Json::Value filenames;
	std::string name;
};
