#pragma once

#include <SDL.h>
#include <optional>
#include "concept.h"
#include "interface.h"

using WorldPoint=SDL_Point;
using ViewPoint=SDL_Point;
using ScreenPoint=SDL_Point;

class Viewport
{
public:
	Viewport();
	WorldPoint Origin() const; // where the viewport's origin (not the world's origin) lands in world coordinates
	ViewPoint Translate(WorldPoint point) const; // convert a point in world coordinates to viewport coordinates
	ScreenPoint Screen(ViewPoint point) const; // convert a point in viewport coordinates to screen coordinates
	void Unpin();
	void Update();
protected:
	int width;
	int height;
	WorldPoint origin;
	WorldPoint position; // where the center of the viewport lands in world coordinates
	std::optional<const Interface::Movable*> pin;

public:
	void Pin(const Interface::Movable *entity) { pin=entity; }
};

class Render
{
public:
	Render();
	void Erase();
	void Flip();
protected:
	void Draw(const SDL_Texture &texture,const SDL_Point &position,const double &rotation);

public:
	template<Concept::Drawable T> void Draw(const Viewport &viewport,const T entity) { Draw(entity->Texture(),viewport.Screen(viewport.Translate(entity->Position())),entity->Rotation()); }
};
