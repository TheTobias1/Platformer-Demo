#pragma once

#include <vector>
#include "Body.h"
#include "SDL.h"

void InitialiseLevel(SDL_Renderer* rend);
std::vector<Body*> GetLevelPlatforms();
int GetNumPlatforms();
void DrawLevel(SDL_Renderer* rend);
void UnloadLevel();