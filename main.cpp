#include <SDL.h>
#include <memory>
#include <stdexcept>
#include <unordered_map>
#include <string>
#include "global.h"
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
		SDL_Event event={0};
		while (live)
		{
			while (SDL_PollEvent(&event))
			{
				switch (event.type)
				{
				case SDL_KEYDOWN:
					state->KeyDown(event.key.keysym.sym);
					break;
				case SDL_KEYUP:
					state->KeyUp(event.key.keysym.sym);
					break;
				case SDL_QUIT:
					SDL_Quit();
					return;
				}
				event={0};
			}

			Update();
			Draw();
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
