#pragma once

#include <chrono>
#include "texture.h"
#include "interface.h"

class State;

class Actor : public Interface::Drawable,public Interface::Movable
{
public:
	Actor(const std::string &textureName,const State *state);
	virtual void Update()=0;
	const SDL_Point& Position() const override;
	const int Rotation() const override;
	const SDL_Texture& Texture() const override;
protected:
	class Texture texture;
	const State *state;
	SDL_Point position;
	double rotation;
};

enum class Roll
{
	NONE,
	RIGHT,
	HALF_RIGHT,
	FULL_RIGHT,
	LEFT,
	HALF_LEFT,
	FULL_LEFT
};

class Player : public Actor
{
public:
	Player(const State *state);
	void DetermineRoll();
	void Update() override;
	const SDL_Texture& Texture() const override;
protected:
	class Texture texture_half_roll_right;
	class Texture texture_full_roll_right;
	class Texture texture_half_roll_left;
	class Texture texture_full_roll_left;
	enum Roll roll;
	std::chrono::time_point<std::chrono::steady_clock> rollLastUpdateTime;
	std::chrono::duration<long long,std::nano> rollTimeElapsed;
	std::chrono::time_point<std::chrono::steady_clock> pitchLastUpdateTime;
	std::chrono::duration<long long,std::nano> pitchTimeElapsed;
};

class Asteroid : public Actor
{
public:
	Asteroid(int x,int y,const State* state);
	void Update() override;
	const SDL_Texture& Texture() const override;
protected:
	Animation::PingPongAdapter pingPongTexture;
	SDL_Texture *currentFrame;
};
