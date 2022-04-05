#pragma once

#include <SDL.h>
#include <SDL_image.h>

inline SDL_Window *window=nullptr;
inline SDL_Renderer *renderer=nullptr;

inline bool InitializeImageSubsystem()
{
	int flags=IMG_INIT_PNG;
	if ((IMG_Init(flags)&flags) != flags) return false;
	return true;
}