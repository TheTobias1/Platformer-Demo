#pragma once

#include "SDL.h"

void InitialiseSpec(SDL_Renderer* rend, int x, int y, int flagX, int flagY);
void UpdateSpec();
void DrawSpec(SDL_Renderer* rend);
void FreeSpec();