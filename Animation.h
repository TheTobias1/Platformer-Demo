#pragma once

#include "SDL.h"

struct SpriteAnimation
{
	int id;
	SDL_Rect* frames;
	int numFrames;
	int frameLength;
	bool loop;
};

void AnimationStep(SpriteAnimation* curAnim, Uint32* nextFrameStamp, int* currentFrame);
void PlayAnimation(SpriteAnimation* anim, SpriteAnimation* curAnim, bool force, Uint32* nextFrameStamp, int* currentFrame);