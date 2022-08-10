#include "render.h"
#include "global.h"

Render::Render()
{
	Erase();
}

void Render::Erase()
{
	SDL_SetRenderDrawColor(renderer,0,0,0,0);
	SDL_RenderClear(renderer);
}

void Render::Draw(const SDL_Texture &texture,const SDL_Point &position,const double &rotation)
{
	SDL_Point size;
	SDL_QueryTexture(const_cast<SDL_Texture*>(&texture),NULL,NULL,&size.x,&size.y);
	SDL_Rect destinationRect({position.x-size.x/2,position.y-size.y/2,size.x,size.y});
	SDL_RenderCopyEx(renderer,const_cast<SDL_Texture*>(&texture),nullptr,&destinationRect,rotation,nullptr,SDL_FLIP_NONE);
}

void Render::Flip()
{
	SDL_RenderPresent(renderer);
}

Viewport::Viewport() : position({0,0})
{
	SDL_GetRendererOutputSize(renderer,&width,&height);
}

WorldPoint Viewport::Origin() const
{
	return origin;
}

ViewPoint Viewport::Translate(WorldPoint point) const
{
	point.x-=Origin().x;
	point.y-=Origin().y;
	return point;
}

ScreenPoint Viewport::Screen(ViewPoint point) const
{
	int width=0;
	int height=0;
	SDL_GetRendererOutputSize(renderer,&width,&height); // TODO: can the number of calls to this be reduced?
	return {
		point.x,
		height-point.y
	};
	// in viewport coordinates, y is distance from bottom of viewport
	// in screen coordinates, y is distance from top of screen
	// so we need the difference between y in viewport coordinates and viewport max height
	// to get y in screen coordinates
}

void Viewport::Unpin()
{
	pin.reset();
}

void Viewport::Update()
{
	if (pin)
		origin={(*pin)->Position().x-width/2,(*pin)->Position().y-height/2};
	else
		origin={position.x-width/2,position.y-height/2};
}
