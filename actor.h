#pragma once

#include <chrono>
#include "texture.h"

class State;

enum class AnimationMethod
{
	STATIC,
	CYCLE,
	PINGPONG
};

class Actor
{
public:
	Actor(const std::string &textureName,const State *state);
	virtual void Draw()=0;
	virtual void Update()=0;
protected:
	Texture texture;
	const State *state;
	std::vector<SDL_Texture*>::const_iterator currentFrame;
	std::vector<SDL_Texture*>::const_reverse_iterator currentFrameReversed;
	std::chrono::time_point<std::chrono::steady_clock> frameLastUpdateTime;
	std::chrono::duration<long long,std::nano> frameTimeElapsed;
	AnimationMethod animationMethod;
	bool animationForward;
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
	void Draw() override;
protected:
	Texture texture_half_roll_right;
	Texture texture_full_roll_right;
	Texture texture_half_roll_left;
	Texture texture_full_roll_left;
	enum Roll roll;
	double angle;
	std::chrono::time_point<std::chrono::steady_clock> rollLastUpdateTime;
	std::chrono::duration<long long,std::nano> rollTimeElapsed;
	std::chrono::time_point<std::chrono::steady_clock> pitchLastUpdateTime;
	std::chrono::duration<long long,std::nano> pitchTimeElapsed;
};

class Asteroid : public Actor
{
public:
	Asteroid(const State *state);
	void Update() override;
	void Draw() override;
};
