#include "SDL.h"
#include "SDL_image.h"
#include "Spec.h"
#include "Body.h"
#include "PlayerCharacter.h"
#include "PlayerManager.h"
#include "Announcement.h"
#include <iostream>
#include <cmath>
using namespace std;

SDL_Rect spawnRect;
SDL_Rect flagRect;
int curPlayer = -1;

SDL_Texture* specImage;
SDL_Texture* flagImage;

Uint32 scoreStamp = 0;
bool canScore = false;

void InitialiseSpec(SDL_Renderer* rend, int specX, int specY, int flagX, int flagY)
{
	canScore = false;

	//Image
	SDL_Surface* specSurface = IMG_Load("art/Spec.png");

	if (specSurface == NULL)
		std::cout << "NO IMAGE FOUND" << std::endl;

	specImage = SDL_CreateTextureFromSurface(rend, specSurface);
	SDL_FreeSurface(specSurface);

	SDL_Surface* flagSurface = IMG_Load("art/Flag2.png");

	if (flagSurface == NULL)
		std::cout << "NO IMAGE FOUND" << std::endl;

	flagImage = SDL_CreateTextureFromSurface(rend, flagSurface);
	SDL_FreeSurface(flagSurface);

	//Position
	spawnRect.x = specX;
	spawnRect.y = specY;
	spawnRect.w = 21;
	spawnRect.h = 21;

	flagRect.x = flagX;
	flagRect.y = flagY;
	flagRect.w = 42;
	flagRect.h = 42;
}

void AttachSpec(int playerNum)
{
	curPlayer = playerNum;

	if (scoreStamp == 0)
	{
		scoreStamp = SDL_GetTicks() + 20000;
		SpawnAnnouncement(544, 20, "text/InstructionText.png", 1500, AnnouncementType::HoldSpec);
	}
	else if(scoreStamp > SDL_GetTicks())
	{
		SpawnAnnouncement(544, 20, "text/InstructionText.png", 1500, AnnouncementType::ScoreTheSpec);
	}
	else
	{
		SpawnAnnouncement(544, 20, "text/InstructionText.png", 1500, AnnouncementType::HoldSpec);
	}
}

void DetachSpec()
{
	SpawnAnnouncement(544, 20, "text/InstructionText.png", 1500, AnnouncementType::CaptureSpec);
	curPlayer = -1;
}

void ScoreSpec(int playerNum)
{
	curPlayer = -2;
	cout << "SCORE" << endl;
}

void UpdateSpec()
{
	//Spec held
	if (curPlayer >= 0)
	{
		PlayerCharacter* p = GetPlayer(curPlayer);

		if (p == nullptr)
		{
			DetachSpec();
		}
		else if(p->dying)
		{
			DetachSpec();
		}

		if (OverlapRect(flagRect, p->playerRect) && canScore && scoreStamp != 0)
		{
			ScoreSpec(curPlayer);
		}
	}
	else if(curPlayer != -2)//Spec Free
	{
		for (int i = 0; i < MAX_PLAYERS; ++i)
		{
			PlayerCharacter* p = GetPlayer(i);
			if (p != nullptr)
			{
				if (!p->dying && OverlapRect(p->playerRect, spawnRect))
				{
					AttachSpec(i);
				}
			}
		}

	}

	if (!canScore && SDL_GetTicks() > scoreStamp && scoreStamp != 0)
	{
		canScore = true;
		if (curPlayer >= 0)
			SpawnAnnouncement(544, 20, "text/InstructionText.png", 1500, AnnouncementType::ScoreTheSpec);
	}
}

void DrawSpec(SDL_Renderer* rend)
{
	#pragma region Spec

	SDL_Rect r = spawnRect;

	if (curPlayer >= 0)
	{
		PlayerCharacter* p = GetPlayer(curPlayer);
		if (p != nullptr)
		{
			r.x = p->playerRect.x + 10 + p->direction;
			r.y = p->playerRect.y - 48;
		}
	}

	float t = (float)SDL_GetTicks() / 60;
	int offset = roundf(sin(t) * 2);

	r.y += offset;

	if (curPlayer != -2)
		SDL_RenderCopy(rend, specImage, NULL, &r);

	#pragma endregion

	#pragma region Flag

	if (canScore)
	{
		SDL_Rect flagImgRect;
		flagImgRect.x = (offset > 0) ? 0 : 21;
		flagImgRect.y = 0;
		flagImgRect.w = 21;
		flagImgRect.h = 21;

		SDL_RenderCopy(rend, flagImage, &flagImgRect, &flagRect);
	}
	


	#pragma endregion


}

void FreeSpec()
{
	SDL_DestroyTexture(specImage);
	SDL_DestroyTexture(flagImage);
}