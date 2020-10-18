#include "Animation.h"
#include "SDL.h"
#include <iostream>

void PlayAnimation(SpriteAnimation* anim, SpriteAnimation* curAnim, bool force, Uint32* nextFrameStamp, int* currentFrame)
{
    if (curAnim != nullptr)
    {
        if (curAnim->id == anim->id && !force)
        {
            return;
        }

        *curAnim = *anim;
    }


    *nextFrameStamp = 0;
    *currentFrame = 0;
}

void AnimationStep(SpriteAnimation* curAnim, Uint32* nextFrameStamp, int* currentFrame)
{
    if (SDL_GetTicks() <= *nextFrameStamp)
        return;

    if (curAnim != nullptr)
    {
        if (*currentFrame >= curAnim->numFrames - 1)
        {
            if (curAnim->loop)
                *currentFrame = 0;
        }
        else
        {
            *currentFrame += 1;
        }

        *nextFrameStamp = SDL_GetTicks() + curAnim->frameLength;
    }

}