#include "state.h"
#include "globals.h"

SDL_Point Viewport::Translate(SDL_Point point)
{
	int width=0;
	int height=0;
	SDL_GetRendererOutputSize(renderer,&width,&height); // TODO: can the number of calls to this be reduced?
	if (pin)
	{
		point.x=point.x-(*pin)->Position().x+width/2;
		point.y=point.y-(*pin)->Position().y+height/2;
	}
	else
	{
		point.x-=center.x-width/2; // NOTE: can we flip this during the last mile so rest of system can assume top right quadrant is +,+?
		point.y+=center.y+height/2;
	}
	return point;
}

void Viewport::Pin(const Actor &actor)
{
	pin=&actor;
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
	viewport.Pin(player);
	actors.push_back(&asteroid);
}

void TestState::Update()
{
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
		SDL_Rect destinationRect({position.x-size.x/2,position.y-size.y/2,size.x,size.y});
		SDL_RenderCopyEx(renderer,const_cast<SDL_Texture*>(&texture),nullptr,&destinationRect,actor->Rotation(),nullptr,SDL_FLIP_NONE);
	}
}
