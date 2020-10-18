#pragma once
#include "SDL.h"

void RunGame();

SDL_Window* GetWindow();
SDL_Renderer* GetRenderer();
SDL_Surface* GetWindowSurface();
int GetDeltaTime();
void FreeMemory();
void QuitGame();