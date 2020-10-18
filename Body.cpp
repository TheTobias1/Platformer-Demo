#include "Body.h"
#include "SDL.h"
#include <vector>
#include <iostream>
using namespace std;

Body::Body(int posX, int posY, int width, int height)
{
	x = posX;
	y = posY;
	w = width;
	h = height;
	prevX = x;
	prevY = y;
	velX = 0;
	velY = 0;
}

vector<int> Body::Position()
{
	int pos[] = { x, y };
	return vector<int>(pos, pos + 2);
}
vector<int> Body::PreviousPosition()
{
	int pos[] = { prevX, prevY };
	return vector<int>(pos, pos + 2);
}
vector<int> Body::Scale()
{
	int scale[] = { w, h };
	return vector<int>(scale, scale + 2);
}
vector<int> Body::Velocity()
{
	int vel[] = { velX, velY };
	return vector<int>(vel, vel + 2);
}

void Body::AddForce(int xForce, int yForce)
{
	velX += xForce;
	velY += yForce;
}
void Body::SetVelocity(int xVel, int yVel)
{
	velX = xVel;
	velY = yVel;
}
void Body::SetPosition(int xPos, int yPos)
{
	prevX = x;
	prevY = y;
	x = xPos;
	y = yPos;
}
void Body::ApplyVelocity(int xVel, int yVel)
{
	SetPosition(x + xVel, y + yVel);
}

void Body::Draw(SDL_Renderer* rend)
{
	SDL_Rect platformRect;
	SDL_SetRenderDrawColor(rend, 250, 250, 250, 255);

	platformRect.x = x;
	platformRect.y = y;
	platformRect.w = w;
	platformRect.h = h;

	SDL_RenderDrawRect(rend, &platformRect);
}

bool OverlapRect(SDL_Rect a, SDL_Rect b)
{
	if (a.x > b.x + b.w || a.x + a.w < b.x || a.y > b.y + b.h || b.y > a.y + a.w)
	{
		return false;
	}

	cout << "Overlap" << endl;
}