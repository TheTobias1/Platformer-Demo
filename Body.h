#pragma once
#include "SDL.h"
#include <vector>

class Body
{
public:
	int x, y;
	int prevX, prevY;
	int w, h;
	int velX, velY;

	Body(int posX, int posY, int width, int height);
	std::vector<int> Position();
	std::vector<int> PreviousPosition();
	std::vector<int> Scale();
	std::vector<int> Velocity();
	void AddForce(int xForce, int yForce);
	void SetVelocity(int xVel, int yVel);
	void SetPosition(int xPos, int yPos);
	void ApplyVelocity(int xVel, int yVel);

	void Draw(SDL_Renderer* rend);
};

bool OverlapRect(SDL_Rect a, SDL_Rect b);