#pragma once

#include "global.h"
#include "interface.h"
#include "texture.h"

class Star : public Interface::Drawable, public Interface::Movable // but stars aren't movable?
{
public:
	Star();
	virtual ~Star();
	const SDL_Texture& Texture() const override;
	SDL_Point Position() const override;
	const int Rotation() const override;
protected:
	class Texture *texture;
	SDL_Texture *rawTexture;
	SDL_Point position;
};

class Starfield
{
public:
	Starfield();
	~Starfield();
	const std::vector<Star*>& Stars() const;
protected:
	std::vector<Star*> stars;
};
