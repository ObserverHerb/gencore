#include "actor.h"
#include "state.h"

#include <cmath>

Actor::Actor(const std::string &textureName,const State *state) : texture(textureName), state(state), position({0,0}), rotation(0.0f)
{
}

SDL_Point Actor::Position() const
{
	return {static_cast<int>(std::lround(position.x)),static_cast<int>(std::lround(position.y))}; // TODO: how big of a deal are rounding errors here?
}

const int Actor::Rotation() const
{
	return rotation;
}

const SDL_Texture& Actor::Texture() const
{
	return *texture.Fetch().front();
}

Player::Player(const State *state) : Actor("Player Ship",state),
	texture_half_roll_right("Player Ship Half Roll Right"),
	texture_full_roll_right("Player Ship Full Roll Right"),
	texture_half_roll_left("Player Ship Half Roll Left"),
	texture_full_roll_left("Player Ship Full Roll Left"),
	roll(Roll::NONE)
{
	rollLastUpdateTime=std::chrono::steady_clock::now();
	rollTimeElapsed=std::chrono::duration<long long,std::nano>::zero();
	pitchLastUpdateTime=std::chrono::steady_clock::now();
	pitchTimeElapsed=std::chrono::duration<long long,std::nano>::zero();

	movementTimestamp=std::chrono::steady_clock::now();
	speed=Convert::Speed<std::chrono::milliseconds>(1,std::chrono::milliseconds(50));
	acceleration=Convert::Acceleration<std::chrono::milliseconds>(1,std::chrono::milliseconds(50));

	position={100,100};
}

void Player::DetermineRoll()
{
	if ((!state->Keys().at(SDLK_RIGHT) && !state->Keys().at(SDLK_LEFT)) || (state->Keys().at(SDLK_RIGHT) && state->Keys().at(SDLK_LEFT)))
	{
		rollTimeElapsed=std::chrono::duration<long long>::zero();
		pitchTimeElapsed=std::chrono::duration<long long>::zero();
		roll=Roll::NONE;
		return;
	}

	bool updateRoll=false;
	if (rollTimeElapsed == std::chrono::duration<long long>::zero())
	{
		updateRoll=true;
	}
	else
	{
		if (rollTimeElapsed > std::chrono::seconds(1))
		{
			updateRoll=true;
		}
	}
	rollTimeElapsed=std::chrono::steady_clock::now()-rollLastUpdateTime;

	bool updateAngle=false;
	if (pitchTimeElapsed == std::chrono::duration<long long>::zero())
	{
		updateAngle=true;
	}
	else
	{
		if (pitchTimeElapsed > std::chrono::milliseconds(10))
		{
			updateAngle=true;
		}
	}
	pitchTimeElapsed=std::chrono::steady_clock::now()-pitchLastUpdateTime;

	if (state->Keys().at(SDLK_RIGHT))
	{
		if (updateRoll)
		{
			roll=Roll::HALF_RIGHT;
			rollLastUpdateTime=std::chrono::steady_clock::now();
		}

		if (updateAngle)
		{
			rotation=rotation == 359 ? 0 : rotation+=1.0f;
			pitchLastUpdateTime=std::chrono::steady_clock::now();
		}
	}

	if (state->Keys().at(SDLK_LEFT))
	{
		if (updateRoll)
		{
			roll=Roll::HALF_LEFT;
			rollLastUpdateTime=std::chrono::steady_clock::now();
		}

		if (updateAngle)
		{
			rotation=rotation == 0 ? 359 : rotation-=1.0f;
			pitchLastUpdateTime=std::chrono::steady_clock::now();
		}
	}
}

void Player::DeterminePosition()
{
	Time::Interval elapsed=Time::Capture(movementTimestamp);

	if (state->Keys().at(SDLK_UP))
	{
		speed+=acceleration*elapsed.count();
	}

	if (state->Keys().at(SDLK_DOWN))
	{
		speed+=(acceleration*-1)*elapsed.count();
		if (speed < 0) speed=0;
	}

	double distance=Convert::Distance(speed,elapsed);
	position.x+=std::sin(Convert::Radians(rotation))*distance;
	position.y+=std::cos(Convert::Radians(rotation))*distance;
}

void Player::Update()
{
	DetermineRoll();
	DeterminePosition();
}

const SDL_Texture& Player::Texture() const
{
	const class Texture *currentTexture=&texture;
	switch (roll)
	{
	case Roll::NONE:
		currentTexture=&texture;
		break;
	case Roll::RIGHT:
		throw std::logic_error("How much right?");
	case Roll::HALF_RIGHT:
		currentTexture=&texture_half_roll_right;
		break;
	case Roll::FULL_RIGHT:
		currentTexture=&texture_full_roll_right;
		break;
	case Roll::LEFT:
		throw std::logic_error("How much left?");
	case Roll::HALF_LEFT:
		currentTexture=&texture_half_roll_left;
		break;
	case Roll::FULL_LEFT:
		currentTexture=&texture_full_roll_left;
		break;
	}
	return *currentTexture->Fetch().front();
}

Asteroid::Asteroid(int x,int y,const State *state) : Actor("DatBoi Asteroid",state), pingPongTexture(texture.Fetch(),std::chrono::milliseconds(200))
{
	position={x,y};
	currentFrame=pingPongTexture();
}

void Asteroid::Update()
{
	currentFrame=pingPongTexture();
}

const SDL_Texture& Asteroid::Texture() const
{
	return *currentFrame;
}
