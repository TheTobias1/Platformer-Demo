#include "SDL.h"
#include "SDL_image.h"
#include <iostream>
#include "Animation.h"
#include "PlayerAnimations.h"
#include "PlayerCharacter.h"
#include "GameLoop.h"
#include "Body.h"
#include "InputManager.h"
#include "Level.h"
#include "PlayerManager.h"

using namespace std;

PlayerCharacter::PlayerCharacter(int startX, int startY, int playerNum) : Body(startX, startY, 32, 32)
{
    playerNumber = playerNum;
    direction = playerNum == 0 ? 1 : -1;

    //input
    releasedJump = true;
    input = GetPlayerInput(playerNum);

    freeFloating = false;

    //sprites
    SDL_Surface* playerSurface = IMG_Load(playerNum == 0? "art/BlueRobot.png" : "art/OrangeRobot.png");

    if (playerSurface == NULL)
        std::cout << "NO IMAGE FOUND" << std::endl;

    playerSpritesheet = SDL_CreateTextureFromSurface(GetRenderer(), playerSurface);
    SDL_FreeSurface(playerSurface);

    //animation
    currentAnimation = new SpriteAnimation();
    PlayAnimation(GetIdleAnimation(), currentAnimation, true, &nextFrameStamp, &currentFrame);
}

PlayerCharacter::~PlayerCharacter()
{
    SDL_DestroyTexture(playerSpritesheet);
    delete currentAnimation;
    currentAnimation = nullptr;
}

bool PlayerCharacter::ResolveCollision(Body collidingBody)
{
    //New Positions
    int bottom = y + h;
    int right = x + w;
    int collidingBottom = collidingBody.y + collidingBody.h;
    int collidingRight = collidingBody.x + collidingBody.w;

    //Old positions
    int oldBottom = prevY + h;
    int oldRight = prevX + w;
    int collidingOldBottom = collidingBody.y + collidingBody.h;
    int collidingOldRight = collidingBody.x + collidingBody.w;

    Body* bBuffer = nullptr;
    Body* tBuffer = nullptr;
    Body* lBuffer = nullptr;
    Body* rBuffer = nullptr;
    bool colliding = true;


    if (bottom < collidingBody.y || y > collidingBottom || x > collidingRight || x + w < collidingBody.x)
    {
        //no collision
        colliding = false;
    }
    else if (bottom >= collidingBody.y && oldBottom < collidingBody.prevY) 
    {
        y = collidingBody.y - h - 1;
        bBuffer = &collidingBody;
    }
    else if (y <= collidingBottom && prevY > collidingOldBottom) 
    {
        if (velY < 0)
        {
            velY = -velY;

            y = collidingBottom + 1;
            tBuffer = &collidingBody;
        }
    }
    else if (right >= collidingBody.x && oldRight < collidingBody.prevX) 
    {
        velX = 0;
        x = collidingBody.x - w - 1;
        rBuffer = &collidingBody;
    }
    else if (x <= collidingRight && prevX > collidingOldRight ) 
    {
        velX = 0;
        x = collidingRight + 1;
        lBuffer = &collidingBody;
    }

    bottomCollision = bBuffer;
    topCollision = tBuffer;
    leftCollision = lBuffer;
    rightCollision = rBuffer;

    return colliding;
}

void PlayerCharacter::Draw(SDL_Renderer* rend, bool blue)
{
    SDL_Rect drawRect;

    drawRect.x = x - 16;
    drawRect.y = y + 1 - 32;
    drawRect.w = 64;
    drawRect.h = 64;

    SDL_Point center;
    center.x = drawRect.x + 32;
    center.y = drawRect.y + 32;

    //--Debug--
    //SDL_SetRenderDrawColor(rend, blue ? 0 : 255, 0, blue ? 255 : 0, 255);
    //SDL_RenderFillRect(rend, &playerRect);
    //---------

    if (currentAnimation != nullptr)
    {
        SDL_Rect frame = currentAnimation->frames[currentFrame];
        SDL_RendererFlip flip = direction == 1? SDL_FLIP_NONE : SDL_FLIP_HORIZONTAL;

        SDL_RenderCopyEx(rend, playerSpritesheet, &frame, &drawRect, 0, &center, flip);
    }

    AnimationStep(currentAnimation, &nextFrameStamp, &currentFrame);
   
}

void PlayerCharacter::Move(int xVel, int yVel)
{
    //Store movement values
    ApplyVelocity(xVel, yVel);
    velX = xVel;
    velY = yVel;

    bool floating = true;
    Body * gBuffer = nullptr;

    vector<Body*> platforms = GetLevelPlatforms();
    int platformCount = GetNumPlatforms();

    for (int i = 0; i < 4; ++i) //4 iterations
    {
        for (int j = 0; j < platformCount; ++j) //go through every platform
        {
            if (platforms[j] != nullptr) //if not null
            {
                Body curPlatform = *platforms[j];
                bool noCol = ResolveCollision(curPlatform);

                if (i == 3 && gBuffer == nullptr)
                {
                    if (GroundCheck(curPlatform))
                    {
                        gBuffer = platforms[j];
                    }
                }


                if (!noCol)
                    floating = false;
            }
        }
    }

    freeFloating = floating;
    groundBuffer = gBuffer;
}

bool PlayerCharacter::GroundCheck(Body curBody)
{
    //positions
    int bottom = y + h + 2;
    int right = x + w;
    int bodyBottom = curBody.y + curBody.h;
    int bodyRight = curBody.x + curBody.w;

    //if in line with body horizontally
    if (right - 2 > curBody.x && x + 2 < bodyRight)
    {
        //touching
        if (bottom > curBody.y && y < curBody.y)
        {
            return true;
        }
    }

    return false;
}

void PlayerCharacter::UpdatePlayer()
{
    if (x + w < 0)
        x = 1080;

    if (x > 1080)
        x = 0;

    if (y > 800)
        Hit();

    //Update the player rect
    playerRect.x = x;
    playerRect.y = y + 1;
    playerRect.w = w;
    playerRect.h = h;

    if (dying)
    {
        PlayAnimation(GetDeathAnimation(), currentAnimation, false, &nextFrameStamp, &currentFrame);
        
        if (SDL_GetTicks() > deathStamp)
        {
            DespawnPlayer(playerNumber, true);
        }

        return;
    }

    if (!input->jump)
    {
        releasedJump = true;
    }

    #pragma region Attack
    if (input->attack && SDL_GetTicks() > nextAttack && !attacking)
    {
        PlayAnimation(GetAttackAnimation(), currentAnimation, false, &nextFrameStamp, &currentFrame);

        attacking = true;
        nextAttack = SDL_GetTicks() + timeBetweenAttacks;
        attackEndStamp = SDL_GetTicks() + attackTime;
        hitboxStamp = SDL_GetTicks() + attackDelay;
        windup = true;

        //Cancel the dash and clamp to other players
        if (dashing)
        {
            CancelDash(moveSpeed);

            SDL_Rect dashRect;
            dashRect.y = y;
            dashRect.w = dashClampRange;
            dashRect.h = 64;

            if (direction == 1)
            {
                dashRect.x = x;
            }
            else
            {
                dashRect.x = x - dashClampRange / 2;
            }

            PlayerCharacter* otherPlayer = GetOtherPlayer(playerNumber);

            if (otherPlayer != nullptr)
            {
                if (OverlapRect(dashRect, otherPlayer->playerRect))
                {
                    cout << "Clamping dash" << endl;

                    if (direction == 1)
                    {
                        x = otherPlayer->x - w;
                    }
                    else
                    {
                        x = otherPlayer->x + w;
                    }
                }
            }

            Move(direction * 10, 0);
        }

        if (velY < 0)
            velY = 0;

        velX = 0;
    }

    if (attacking)
    {
        if (windup && SDL_GetTicks() > hitboxStamp)
        {
            //HIT
            SDL_Rect hitRect;
            hitRect.y = y;
            hitRect.w = hitRange;
            hitRect.h = 64;
            
            if (direction == 1)
            {
                hitRect.x = x;
            }
            else
            {
                hitRect.x = x - hitRange / 2;
            }

            PlayerCharacter* otherPlayer = GetOtherPlayer(playerNumber);

            if (otherPlayer != nullptr)
            {
                if (OverlapRect(hitRect, otherPlayer->playerRect))
                {

                    otherPlayer->Hit();
                }
            }

            windup = false;
        }

        if (SDL_GetTicks() > attackEndStamp)
        {
            attacking = false;
        }
    }
    #pragma endregion

    #pragma region Dash

    if (SDL_GetTicks() > nextDash && input->dash && input->direction != 0 && !attacking)
    {
        PlayAnimation(GetDashAnimation(), currentAnimation, false, &nextFrameStamp, &currentFrame);

        std::cout << "Dash" << std::endl;
        dashing = true;
        dashStamp = SDL_GetTicks() + dashTime;

        if (input->direction > 0)
        {
            dashVel = dashSpeed;
        }
        else
        {
            dashVel = -dashSpeed;
        }
    }

    if (dashing)
    {
        if (SDL_GetTicks() > dashStamp || leftCollision != nullptr || rightCollision != nullptr || input->attack)
        {
            CancelDash(moveSpeed);
        }
        else
        {
            velX = dashVel;
            velY = 0;
        }
    }
    #pragma endregion

    #pragma region Stomping

    if (velY > 0 && !dashing)
    {
        PlayerCharacter* otherPlayer = GetOtherPlayer(playerNumber);

        if (otherPlayer != nullptr)
        {
            if (!otherPlayer->dying && y + (h - 12) < otherPlayer->y && OverlapRect(playerRect, otherPlayer->playerRect))
            {
                cout << "Stomp" << endl;

                otherPlayer->Hit();
                velY = -doubleJumpYForce;
            }
        }
    }

    #pragma endregion


    #pragma region Movment
    //If grounded
    if (groundBuffer != nullptr)
    {
        hasDoubleJump = true;

        if(!dashing)
        {
            #pragma region Moving

            if (input->direction != 0 && !attacking)
            {
                    
                PlayAnimation(GetRunAnimation(), currentAnimation, false, &nextFrameStamp, &currentFrame);

                //int curVelX = velX;
                velX += acceleration * input->direction;
                int maximum = (SDL_GetTicks() > dampStamp)? moveSpeed : speedLimit;

                if (SDL_GetTicks() > dampStamp)
                {
                    speedLimit = topSpeed;
                }


                if (velX > maximum)
                {
                    velX = maximum;
                }
                else if(velX < -maximum)
                {
                    velX = -maximum;
                }

                direction = input->direction;
            }
            else //decelerate towards 0
            {
                int d = 0;
                if (!attacking)
                {
                    PlayAnimation(GetIdleAnimation(), currentAnimation, false, &nextFrameStamp, &currentFrame);

                    d = deceleration;
                }
                else
                {
                    if (windup)
                        d = 1;
                }
                    

                if (velX > 0)
                {
                    velX -= d;
                    if (velX < 0)
                        velX = 0;
                }
                else if(velX < 0)
                {
                    velX += d;
                    if (velX > 0)
                        velX = 0;
                }
            }

            if (velY > 0)
                velY = 0;

            #pragma endregion

        }

        #pragma region Jumping
        if (input->jump && releasedJump && SDL_GetTicks() > nextJump && !attacking)
        {
            nextJump = SDL_GetTicks() + 200;
            releasedJump = false;

            velY = -jumpForce;

            if (dashing)
            {
                speedLimit = dashSpeed - 2;
                CancelDash(dashSpeed - 2);
                dampStamp = SDL_GetTicks() + 80;
            }

        }
        #pragma endregion

    }
    else//In Air
    {
        #pragma region DoubleJumping
        if (input->jump && releasedJump && SDL_GetTicks() > nextJump && hasDoubleJump && velY > -doubleJumpYForce && !attacking)
        {
            PlayAnimation(GetJumpAnimation(), currentAnimation, true, &nextFrameStamp, &currentFrame);

            if (dashing)
            {
                speedLimit = topSpeed;
                CancelDash(topSpeed);
            }

            nextJump = SDL_GetTicks() + 200;
            releasedJump = false;
            hasDoubleJump = false;

            velY = -doubleJumpYForce;

            //How fast should we go coming out of the jump
            if ((input->direction > 0 && velX > 0) || (input->direction < 0 && velX < 0))
            {
                if (velX > 0)
                    velX = topSpeed;
                else
                    velX = -topSpeed;
            }
            else
            {
                if (input->direction)
                {
                    if (velX > 0)
                        velX = moveSpeed;
                    else
                        velX = -moveSpeed;
                }
 
            }
        }
        #pragma endregion

        if (!dashing)
        {
            if (!attacking)
            {
                PlayAnimation(GetJumpAnimation(), currentAnimation, false, &nextFrameStamp, &currentFrame);

                if(input->direction != 0)
                    direction = input->direction;
            }  

            #pragma region Moving

            velY += gravityStrength;
            if (velY > maxGravity)
                velY = maxGravity;

            int curVelX = velX;
            velX += inAirAcceleration * input->direction;

            //If going slower than the move speed, don't accelerate over the move speed
            if ((curVelX <= moveSpeed || velX < curVelX) && (curVelX >= -moveSpeed || velX > curVelX))
            {
                speedLimit = topSpeed;
                dampStamp = 0;
                if (velX > moveSpeed)
                {
                    velX = moveSpeed;
                }
                else if (velX < -moveSpeed)
                {
                    velX = -moveSpeed;
                }
            }
            else
            {
                dampStamp = SDL_GetTicks() + 80;
                if (velX > speedLimit)
                {
                    velX = speedLimit;
                }
                else if (velX < -speedLimit)
                {
                    velX = -speedLimit;
                }
            }

            #pragma endregion
        }
    }

    Move(velX, velY);

    #pragma endregion
}

void PlayerCharacter::CancelDash(int cancelSpeed)
{
    dashing = false;
    if (dashVel > 0)
    {
        velX = cancelSpeed;
    }
    else
    {
        velX = -cancelSpeed;
    }

    nextDash = SDL_GetTicks() + 1500;
}

/*void PlayerCharacter::PlayAnimation(SpriteAnimation* anim, bool force)
{
    if (currentAnimation != nullptr)
    {
        if (currentAnimation->id == anim->id && !force)
        {
            return;
        }
    }
    
    currentAnimation = anim;
    nextFrameStamp = 0;
    currentFrame = 0;
}

void PlayerCharacter::AnimationStep()
{
    ++currentFrame;
    if (currentFrame >= currentAnimation->numFrames)
    {
        if(currentAnimation->loop)
            currentFrame = 0;
        else
            --currentFrame;
    }

    nextFrameStamp = SDL_GetTicks() + currentAnimation->frameLength;
}*/

void PlayerCharacter::Hit()
{
    if (dying)
        return;

    deathStamp = SDL_GetTicks() + deathDelay;
    dying = true;
}