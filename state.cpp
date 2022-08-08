#include "state.h"
#include "global.h"

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
