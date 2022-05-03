#include "state.h"
#include "globals.h"

SDL_Point Viewport::Translate(SDL_Point point)
{
	point.x-=center.x; // NOTE: why is this subtract instead of add?
	point.y+=center.y;
	return point;
}

void Viewport::Pin(const Actor &actor)
{
	pin=&actor;
}

void Viewport::Update()
{
	if (pin) center=(*pin)->Position();
}

State::State()
{
	keys[SDLK_RIGHT]=false;
	keys[SDLK_LEFT]=false;
}

void State::KeyDown(SDL_Keycode key)
{
	keys[key]=true;
}

void State::KeyUp(SDL_Keycode key)
{
	keys[key]=false;
}

const Keys& State::Keys() const
{
	return keys;
}

TestState::TestState() : State(), player(this), asteroid(this)
{
	actors.push_back(&player);
	//viewport.Pin(player);
	actors.push_back(&asteroid);
}

void TestState::Update()
{
	viewport.Update();
	for (Actor *actor : actors) actor->Update();
}

void TestState::Present()
{
	for (Actor *actor : actors)
	{
		const SDL_Texture &texture=actor->Texture();
		const SDL_Point &position=viewport.Translate(actor->Position());
		SDL_Point size;
		SDL_QueryTexture(const_cast<SDL_Texture*>(&texture),NULL,NULL,&size.x, &size.y);
		SDL_Rect destinationRect({position.x,position.y,size.x,size.y});
		SDL_RenderCopyEx(renderer,const_cast<SDL_Texture*>(&texture),nullptr,&destinationRect,actor->Rotation(),nullptr,SDL_FLIP_NONE);
	}
}
