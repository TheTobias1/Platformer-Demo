#include "SDL.h"
#include "Animation.h"
#include "PlayerAnimations.h"

using namespace std;

SpriteAnimation* idleAnimation = nullptr;
SpriteAnimation* runAnimation = nullptr;
SpriteAnimation* jumpAnimation = nullptr;
SpriteAnimation* dashAnimation = nullptr;
SpriteAnimation* attackAnimation = nullptr;
SpriteAnimation* deathAnimation = nullptr;

void InitialiseAnimations()
{
#pragma region Idle Animation
	{
		SpriteAnimation* anim = new SpriteAnimation();
		SDL_Rect* rects = new SDL_Rect[4];

		for (int i = 0; i < 4; ++i)
		{
			rects[i].x = 32 * i;
			rects[i].y = 0;
			rects[i].w = 32;
			rects[i].h = 32;
		}

		anim->id = 0;
		anim->numFrames = 4;
		anim->loop = true;
		anim->frames = rects;
		anim->frameLength = 100;

		idleAnimation = anim;
	}
#pragma endregion

#pragma region Running Animation
	{
		SpriteAnimation* anim = new SpriteAnimation();
		SDL_Rect* rects = new SDL_Rect[6];

		rects[0].x = 160;
		rects[0].y = 0;

		rects[1].x = 192;
		rects[1].y = 0;

		rects[2].x = 224;
		rects[2].y = 0;

		rects[3].x = 0;
		rects[3].y = 32;

		rects[4].x = 32;
		rects[4].y = 32;

		rects[5].x = 64;
		rects[5].y = 32;

		for (int i = 0; i < 6; ++i)
		{
			rects[i].w = 32;
			rects[i].h = 32;
		}

		anim->id = 1;
		anim->numFrames = 6;
		anim->loop = true;
		anim->frames = rects;
		anim->frameLength = 70;

		runAnimation = anim;
	}
#pragma endregion

#pragma region Jump Animation
	{
		SpriteAnimation* anim = new SpriteAnimation();
		SDL_Rect* rects = new SDL_Rect[5];

		for (int i = 0; i < 5; ++i)
		{
			rects[i].x = 96 + 32 * i;
			rects[i].y = 32;
			rects[i].w = 32;
			rects[i].h = 32;
		}

		anim->id = 2;
		anim->numFrames = 5;
		anim->loop = false;
		anim->frames = rects;
		anim->frameLength = 100;

		jumpAnimation = anim;
	}
#pragma endregion

#pragma region Dash Animation
	{
		SpriteAnimation* anim = new SpriteAnimation();
		SDL_Rect* rects = new SDL_Rect[1];

		rects[0].x = 32;
		rects[0].y = 128;
		rects[0].w = 32;
		rects[0].h = 32;

		anim->id = 3;
		anim->numFrames = 1;
		anim->loop = true;
		anim->frames = rects;
		anim->frameLength = 200;

		dashAnimation = anim;
	}
#pragma endregion

#pragma region AttackAnimation
	{
		SpriteAnimation* anim = new SpriteAnimation();
		SDL_Rect* rects = new SDL_Rect[3];

		for (int i = 0; i < 3; ++i)
		{
			rects[i].x = 96 +32 * i;
			rects[i].y = 64;
			rects[i].w = 32;
			rects[i].h = 32;
		}

		anim->id = 4;
		anim->numFrames = 3;
		anim->loop = false;
		anim->frames = rects;
		anim->frameLength = 70;

		attackAnimation = anim;
	}
#pragma endregion

#pragma region DeathAnimation
	{
		SpriteAnimation* anim = new SpriteAnimation();
		SDL_Rect* rects = new SDL_Rect[5];

		rects[0].x = 160;
		rects[0].y = 96;

		rects[1].x = 192;
		rects[1].y = 96;

		rects[2].x = 224;
		rects[2].y = 96;

		rects[3].x = 0;
		rects[3].y = 128;

		rects[4].x = 64;
		rects[4].y = 128;


		for (int i = 0; i < 5; ++i)
		{
			rects[i].w = 32;
			rects[i].h = 32;
		}

		anim->id = 5;
		anim->numFrames = 5;
		anim->loop = false;
		anim->frames = rects;
		anim->frameLength = 100;

		deathAnimation = anim;
	}
#pragma endregion

}

SpriteAnimation* GetIdleAnimation()
{
	return idleAnimation;
}

SpriteAnimation* GetRunAnimation()
{
	return runAnimation;
}

SpriteAnimation* GetJumpAnimation()
{
	return jumpAnimation;
}

SpriteAnimation* GetDashAnimation()
{
	return dashAnimation;
}

SpriteAnimation* GetAttackAnimation()
{
	return attackAnimation;
}

SpriteAnimation* GetDeathAnimation()
{
	return deathAnimation;
}

void FreeAnimations()
{
	delete	idleAnimation;
	delete	runAnimation;
	delete	jumpAnimation;
	delete	dashAnimation;
	delete	attackAnimation;
	delete	deathAnimation;

	idleAnimation = nullptr;
	runAnimation = nullptr;
	jumpAnimation = nullptr;
	dashAnimation = nullptr;
	attackAnimation = nullptr;
	deathAnimation = nullptr;
}


