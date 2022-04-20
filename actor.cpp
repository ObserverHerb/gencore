#include "globals.h"
#include "actor.h"
#include "state.h"

Actor::Actor(const std::string &textureName,const State *state) : texture(textureName), state(state)
{
}

Player::Player(const State *state) : Actor("Player Ship",state),
	texture_half_roll_right("Player Ship Half Roll Right"),
	texture_full_roll_right("Player Ship Full Roll Right"),
	texture_half_roll_left("Player Ship Half Roll Left"),
	texture_full_roll_left("Player Ship Full Roll Left"),
	roll(Roll::NONE),
	angle(0)
{
	rollLastUpdateTime=std::chrono::steady_clock::now();
	rollTimeElapsed=std::chrono::duration<long long,std::nano>::zero();
	pitchLastUpdateTime=std::chrono::steady_clock::now();
	pitchTimeElapsed=std::chrono::duration<long long,std::nano>::zero();
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
			angle=angle == 359 ? 0 : angle+=1.0f;
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
			angle=angle == 0 ? 359 : angle-=1.0f;
			pitchLastUpdateTime=std::chrono::steady_clock::now();
		}
	}
}

void Player::Update()
{
	DetermineRoll();
}

void Player::Draw()
{
	Texture *currentTexture=&texture;
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
	SDL_Point size;
	SDL_QueryTexture(currentTexture->Fetch().front(),NULL,NULL,&size.x, &size.y);
	size.x*=3; // TODO: restore original size in production
	size.y*=3;
	int width=0;
	int height=0;
	SDL_GetRendererOutputSize(renderer,&width,&height);
	SDL_Rect destinationRect({width/2-size.x/2,height/2-size.y/2,size.x,size.y});
	SDL_RenderCopyEx(renderer,currentTexture->Fetch().front(),nullptr,&destinationRect,angle,nullptr,SDL_FLIP_NONE);
}

Asteroid::Asteroid(const State *state) : Actor("DatBoi Asteroid",state), pingPongTexture(texture.Fetch(),std::chrono::milliseconds(200))
{
}

void Asteroid::Update()
{
}

void Asteroid::Draw()
{
	SDL_Texture *frame=pingPongTexture();
	SDL_Point size;
	SDL_QueryTexture(frame,NULL,NULL,&size.x, &size.y);
	int x=50-size.x/2;
	int y=50-size.y/2;
	size.x*=5; // TODO: restore original size in production
	size.y*=5;
	SDL_Rect destinationRect({x,y,size.x,size.y});
	SDL_RenderCopyEx(renderer,frame,nullptr,&destinationRect,0,nullptr,SDL_FLIP_NONE);
}
