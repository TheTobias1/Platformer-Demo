#include "SDL.h"
#include "SDL_image.h"
#include "Announcement.h"
#include "GameLoop.h"
#include <iostream>

SpriteAnimation* CaptureSpecAnim;
SpriteAnimation* HoldSpecAnim;
SpriteAnimation* ScoreSpecAnim;

Announcement* currentAnnouncement;

SpriteAnimation* currentAnim = nullptr;
Uint32 frameStamp = 0;
int curFrame = 0;

void InitialiseAnnouncements()
{

	currentAnim = new SpriteAnimation();

#pragma region Capture The Spec
	{
		SpriteAnimation* anim = new SpriteAnimation();
		SDL_Rect* rects = new SDL_Rect[3];

		rects[0].x = 2;
		rects[0].y = 215;
		rects[0].w = 628;
		rects[0].h = 105;

		rects[1].x = 632;
		rects[1].y = 215;
		rects[1].w = 279;
		rects[1].h = 105;

		rects[2].x = 910;
		rects[2].y = 215;
		rects[2].w = 358;
		rects[2].h = 105;

		anim->id = 1;
		anim->numFrames = 3;
		anim->loop = false;
		anim->frames = rects;
		anim->frameLength = 500;

		CaptureSpecAnim = anim;
	}
#pragma endregion

#pragma region Hold The Spec
	{
		SpriteAnimation* anim = new SpriteAnimation();
		SDL_Rect* rects = new SDL_Rect[3];

		rects[0].x = 12;
		rects[0].y = 115;
		rects[0].w = 358;
		rects[0].h = 105;

		rects[1].x = 378;
		rects[1].y = 115;
		rects[1].w = 279;
		rects[1].h = 105;

		rects[2].x = 656;
		rects[2].y = 115;
		rects[2].w = 362;
		rects[2].h = 105;

		anim->id = 1;
		anim->numFrames = 3;
		anim->loop = false;
		anim->frames = rects;
		anim->frameLength = 500;

		HoldSpecAnim = anim;
	}
#pragma endregion

#pragma region Score The Spec
	{
		SpriteAnimation* anim = new SpriteAnimation();
		SDL_Rect* rects = new SDL_Rect[3];

		rects[0].x = 12;
		rects[0].y = 16;
		rects[0].w = 454;
		rects[0].h = 105;

		rects[1].x = 467;
		rects[1].y = 16;
		rects[1].w = 281;
		rects[1].h = 105;

		rects[2].x = 750;
		rects[2].y = 16;
		rects[2].w = 357;
		rects[2].h = 105;

		anim->id = 1;
		anim->numFrames = 3;
		anim->loop = false;
		anim->frames = rects;
		anim->frameLength = 500;

		ScoreSpecAnim = anim;
	}
#pragma endregion
}

void ClearAnnouncement()
{
	if (currentAnnouncement == nullptr)
		return;
	else
		delete currentAnnouncement;

	currentAnnouncement = nullptr;
}

Announcement::Announcement(int centX, int y, std::string spriteSheetPath, Uint16 liveTime)
{
	centerX = centX;
	yPos = y;
	deathStamp = SDL_GetTicks() + liveTime;

	SDL_Surface* surface = IMG_Load(spriteSheetPath.c_str());

	if (surface == NULL)
		std::cout << "NO IMAGE FOUND" << std::endl;

	spriteSheet = SDL_CreateTextureFromSurface(GetRenderer(), surface);
	SDL_FreeSurface(surface);
}

Announcement::~Announcement()
{
	SDL_DestroyTexture(spriteSheet);
}

void UpdateAnnouncement(SDL_Renderer* rend)
{
	if (currentAnnouncement == nullptr || currentAnim == nullptr)
	{
		return;
	}

	AnimationStep(currentAnim, &frameStamp, &curFrame);

	SDL_Rect frame = currentAnim->frames[curFrame];

	SDL_Rect drawRect;

	drawRect.x = currentAnnouncement->centerX - (frame.w / 2);
	drawRect.y = currentAnnouncement->yPos;
	drawRect.w = frame.w;
	drawRect.h = frame.h;

	SDL_RenderCopy(rend, currentAnnouncement->spriteSheet, &frame, &drawRect);

	if (SDL_GetTicks() >= currentAnnouncement->deathStamp)
	{
		std::cout << "Announcement died" << std::endl;
		ClearAnnouncement();
	}
	
}

void SpawnAnnouncement(int centX, int y, std::string spriteSheetPath, Uint16 liveTime, AnnouncementType announcementType)
{
	ClearAnnouncement();

	SpriteAnimation* anim = nullptr;

	switch (announcementType)
	{
		case AnnouncementType::CaptureSpec:
			anim = CaptureSpecAnim;
			break;
		case AnnouncementType::HoldSpec:
			anim = HoldSpecAnim;
			break;
		case AnnouncementType::ScoreTheSpec:
			anim = ScoreSpecAnim;
			break;
	}

	if (anim != nullptr)
	{
		std::cout << "Spawning announcement" << std::endl;
		currentAnnouncement = new Announcement(centX, y, spriteSheetPath, liveTime);

		PlayAnimation(anim, currentAnim, true, &frameStamp, &curFrame);
		frameStamp = SDL_GetTicks() + currentAnim->frameLength;
	}
		
}

void FreeAnnouncements()
{
	delete currentAnnouncement;
	delete currentAnim;
}
