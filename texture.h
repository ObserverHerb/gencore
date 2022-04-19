#pragma once

#include <SDL.h>
#include <json/json.h>
#include <unordered_map>
#include <string>
#include <vector>

class Texture
{
public:
	Texture(const std::string &name);
	const std::vector<SDL_Texture*>& Fetch() const;
protected:
	static std::unordered_map<std::string,std::vector<SDL_Texture*>> cache;
	static Json::Value filenames;
	std::string name;
};
