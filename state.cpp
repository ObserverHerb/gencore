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

TestState::TestState() : State(), player(this), asteroid(50,50,this)
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
	Render render;
	for (const Actor *actor : actors) render.Draw(viewport,actor);
	render.Flip();
}
