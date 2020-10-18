#pragma once
#include "Animation.h"

void InitialiseAnimations();
SpriteAnimation* GetIdleAnimation();
SpriteAnimation* GetRunAnimation();
SpriteAnimation* GetJumpAnimation();
SpriteAnimation* GetDashAnimation();
SpriteAnimation* GetAttackAnimation();
SpriteAnimation* GetDeathAnimation();
void FreeAnimations();