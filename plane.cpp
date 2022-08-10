#include "plane.h"

Star::Star() : texture(nullptr)
{
	switch (rand() % 3) // TODO: is there a more "C++" way to do deterministic rand?
	{
	case 0:
		texture=new class Texture("White Star Small");
		break;
	case 1:
		texture=new class Texture("White Star Medium");
		break;
	case 2:
		texture=new class Texture("White Star Large");
		break;
	default:
		throw std::logic_error("More than three star textures requested");
	}
	rawTexture=texture->Fetch().front();

	position.x=(rand()%65535)-65535/2; // TODO: is there a more "C++" way to do deterministic rand?
	position.y=(rand()%65535)-65535/2;
}

Star::~Star()
{
	delete texture;
}

const SDL_Texture& Star::Texture() const
{
	return *rawTexture;
}

SDL_Point Star::Position() const
{
	return position;
}

const int Star::Rotation() const
{
	return 0;
}

Starfield::Starfield()
{
	for (int count=0; count < 65535; count++)
	{
		stars.push_back(new Star());
	}
}

Starfield::~Starfield()
{
	for (Star *star : stars) delete star;
}

const std::vector<Star*>& Starfield::Stars() const { return stars; }
