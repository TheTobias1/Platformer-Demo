#pragma once
#include "SDL.h"
#include <vector>
#include "Animation.h"
#include "Body.h"
#include "InputManager.h"


class PlayerCharacter : public Body
{
private:
	PlayerInput * input;

	Body* groundBuffer = nullptr;
	bool hasDoubleJump = true;
	bool releasedJump;
	Uint32 nextJump = 0;
	Uint32 dampStamp = 0; //Allows a player to maintain in air momentum for a short while

	bool dashing = false;
	Uint32 dashStamp = 0;
	int dashVel = 0;
	Uint32 nextDash = 0;

	bool attacking = false;
	Uint32 hitboxStamp = 0;
	Uint32 attackEndStamp = 0;
	Uint32 nextAttack = 0;
	bool windup = false;

	Uint32 deathStamp;

	int speedLimit = 10;

	bool GroundCheck(Body curBody);
	void CancelDash(int cancelSpeed);
	void Hit();

	//Animation and Drawing
	SDL_Texture* playerSpritesheet;

	int currentFrame = 0;
	Uint32 nextFrameStamp = 0;

	//void PlayAnimation(SpriteAnimation* anim, bool force);
	//void AnimationStep();

	SpriteAnimation* currentAnimation = nullptr;
public:
	int playerNumber;
	bool dying;

	const int moveSpeed = 5;
	const int topSpeed = 7;
	const int acceleration = 2;
	const int inAirAcceleration = 1;
	const int deceleration = 4;
	const int gravityStrength = 1;
	const int jumpForce = 12;
	const int doubleJumpYForce = 12;
	const int dashSpeed = 17;
	const int dashTime = 100;
	const int maxGravity = 16;
	const int attackTime = 260;
	const int attackDelay = 70;
	const int timeBetweenAttacks = 450;
	const int deathDelay = 500;
	const int hitRange = 64;
	const int dashClampRange = 80;

	PlayerCharacter(int startX, int startY, int playerNum);
	~PlayerCharacter();

	Body* bottomCollision = nullptr;
	Body* topCollision = nullptr;
	Body* leftCollision = nullptr;
	Body* rightCollision = nullptr;
	bool freeFloating;

	int direction;

	bool ResolveCollision(Body collidingBody);
	void Draw(SDL_Renderer* rend, bool blue);
	void UpdatePlayer();
	void Move(int xVel, int yVel);

	SDL_Rect playerRect;
};