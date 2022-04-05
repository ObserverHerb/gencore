#include "state.h"

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

TestState::TestState() : State(), player(this)
{
}

void TestState::Update()
{
	player.Update();
}

void TestState::Present()
{
	player.Draw();
}
