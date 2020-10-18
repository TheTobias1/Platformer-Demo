#pragma once
#include "Animation.h"
#include "SDL.h"
#include <string>

enum AnnouncementType{CaptureSpec, HoldSpec, ScoreTheSpec};

void InitialiseAnnouncements();
void SpawnAnnouncement(int centX, int y, std::string spriteSheetPath, Uint16 liveTime, AnnouncementType announcementType);
void UpdateAnnouncement(SDL_Renderer* rend);
void FreeAnnouncements();

class Announcement
{
public:
	Announcement(int centX, int y, std::string spriteSheetPath, Uint16 liveTime);
	~Announcement();

	int centerX;
	int yPos;
	int width;
	int height;
	SDL_Texture* spriteSheet;
	Uint32 deathStamp;
};