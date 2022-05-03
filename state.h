#pragma once
#include <SDL.h>
#include <optional>
#include "actor.h"

using Keys=std::unordered_map<SDL_Keycode,bool>;

class Viewport
{
public:
	Viewport() : center({0,0}) { }
	SDL_Point center;
	SDL_Point Translate(SDL_Point point);
	void Pin(const Actor &actor);
	void Update();
protected:
	std::optional<const Actor*> pin;
};

class State
{
public:
	State();
	void KeyDown(SDL_Keycode key);
	void KeyUp(SDL_Keycode key);
	const ::Keys& Keys() const;
	virtual void Update()=0;
	virtual void Present()=0;
protected:
	::Keys keys;
	Viewport viewport;
};

class TestState : public State
{
public:
	TestState();
	void Update() override;
	void Present() override;
protected:
	Player player;
	Asteroid asteroid;
	std::vector<Actor*> actors;
};
