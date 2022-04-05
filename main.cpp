#include <SDL.h>
#include <memory>
#include <stdexcept>
#include <unordered_map>
#include <string>
#include "globals.h"
#include "state.h"

class Game
{
public:
	Game() : live(true)
	{
		state=new TestState();
	}
	bool Live() { return live; }
	void Draw()
	{
		state->Present();
	}
	void Update()
	{
		state->Update();
	}
	void Loop()
	{
		SDL_Event event;
		while (live)
		{
			SDL_SetRenderDrawColor(renderer,0,0,0,0);
			SDL_RenderClear(renderer);
			Draw();
			SDL_RenderPresent(renderer);

			if (SDL_PollEvent(&event))
			{
				switch (event.type)
				{
				case SDL_KEYDOWN:
					switch (event.key.keysym.sym)
					{
					case SDLK_RIGHT:
						state->KeyDown(SDLK_RIGHT);
						break;
					case SDLK_LEFT:
						state->KeyDown(SDLK_LEFT);
						break;
					}
					break;
				case SDL_KEYUP:
					switch (event.key.keysym.sym)
					{
					case SDLK_RIGHT:
						state->KeyUp(SDLK_RIGHT);
						break;
					case SDLK_LEFT:
						state->KeyUp(SDLK_LEFT);
						break;
					}
					break;
				case SDL_QUIT:
					SDL_Quit();
					return;
				}
			}

			Update();
		}
	}
protected:
	bool live;
	State *state;
};

enum class ExitCode
{
	OK=0,
	RENDERER_INITIALIZATION_FAILED,
	IMAGE_SUBSYSTEM_FAILED
};

int main(int argc, char* argv[])
{
	SDL_Init(SDL_INIT_EVERYTHING);
	if (!InitializeImageSubsystem()) return static_cast<int>(ExitCode::IMAGE_SUBSYSTEM_FAILED);

	window=SDL_CreateWindow("Gencore",SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED,800,600,SDL_WINDOW_SHOWN);

	renderer=SDL_CreateRenderer(window,-1,SDL_RENDERER_SOFTWARE);
	if (!renderer) return static_cast<int>(ExitCode::RENDERER_INITIALIZATION_FAILED);

	Game().Loop();

	IMG_Quit();
	SDL_Quit();

	return 0;
}