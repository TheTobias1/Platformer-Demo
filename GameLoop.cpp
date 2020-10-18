#include "GameLoop.h"
#include "Announcement.h"
#include "InputManager.h"
#include "Level.h"
#include "PlayerCharacter.h"
#include "PlayerAnimations.h"
#include "SDL_image.h"
#include "PlayerManager.h"
#include "Spec.h"
#include <stdlib.h>
#include <iostream>
#include <vector>
using namespace std;

#pragma region Variables
#pragma region Window Variables
SDL_Window* window = nullptr;
SDL_Renderer* renderer = nullptr;
SDL_Surface* windowSurface = nullptr;
#pragma endregion

#pragma region Frame Timing Variables
const int frameTime = 16;
int deltaTime = 16;
#pragma endregion

bool quitGame = false;
#pragma endregion

void InitialiseGame()
{
#pragma region Initialisation

#pragma region SDL Initialisation

	SDL_Init(SDL_INIT_EVERYTHING);
	window = SDL_CreateWindow("ProjectWoods", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1088, 736, SDL_WINDOW_SHOWN);
	renderer = SDL_CreateRenderer(window, -1, 0);
	windowSurface = SDL_GetWindowSurface(window);
	int result = IMG_Init(IMG_INIT_PNG);
	std::cout << "Result: " << result << std::endl;

#pragma endregion

	InitialiseInput();
	InitialiseLevel(renderer);
	InitialiseAnimations();
	InitialiseSpec(renderer, 536, 40, 536, 439);
	InitialiseAnnouncements();

#pragma endregion
}

void RunGame()
{
	InitialiseGame();

	//PlayerCharacter player1(20, 500, 0);
	//PlayerCharacter player2(1000, 500, 1);
	SpawnPlayer(20, 500, 0);
	SpawnPlayer(1000, 500, 1);

	std::cout << "Initialised" << std::endl;

	SpawnAnnouncement(544, 20, "text/InstructionText.png", 1500, AnnouncementType::CaptureSpec);

	while (!quitGame)
	{

		Uint32 frameStart = SDL_GetTicks();
		PollInput();

		#pragma region Game Quiting
		if (QuitPressed())
		{
			QuitGame();
			std::cout << "QUITING" << std::endl;
		}

		#pragma endregion

		#pragma region Game Logic

		CheckRespawn();

		//Update Players
		if (GetPlayer(0) != nullptr)
		{
			GetPlayer(0)->UpdatePlayer();
		}

		if (GetPlayer(1) != nullptr)
		{
			GetPlayer(1)->UpdatePlayer();
		}

		UpdateSpec();

		#pragma endregion


		#pragma region Rendering

		//Black Screen
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderClear(renderer);

		DrawLevel(renderer);

		#pragma region Rendering Platforms

		int platformCount = GetNumPlatforms();
		vector<Body*> platforms = GetLevelPlatforms();

		//Draw all platforms
		for (int i = 0; i < platformCount; ++i)
		{
			if (platforms[i] != nullptr)
			{
				Body curPlatform = *platforms[i];

				//Draw the platform
				curPlatform.Draw(renderer);
			}
		}

		#pragma endregion

		DrawSpec(renderer);

		#pragma region Rendering Player

		if (GetPlayer(0) != nullptr)
			GetPlayer(0)->Draw(renderer, true);

		if (GetPlayer(1) != nullptr)
			GetPlayer(1)->Draw(renderer, false);

		#pragma endregion

		UpdateAnnouncement(renderer);

		//Draw on screen
		SDL_RenderPresent(renderer);
		#pragma endregion


		#pragma region Frame Delay
		//wait for remaining time
		Uint32 currentFrameTime = SDL_GetTicks() - frameStart;
		if (currentFrameTime < frameTime)
		{
			SDL_Delay(frameTime - currentFrameTime);
		}
		deltaTime = SDL_GetTicks() - frameStart;
		#pragma endregion
	}

	FreeMemory();

	SDL_Quit();
}

SDL_Window* GetWindow() { return window; }
SDL_Renderer* GetRenderer() { return renderer; }
SDL_Surface* GetWindowSurface() { return windowSurface; }
int GetDeltaTime() { return deltaTime; }

void QuitGame()
{
	quitGame = true;
}

void FreeMemory()
{
	FreeAnimations();
	UnloadLevel();
	FreeAnnouncements();
	FreeInput();
	FreePlayers();
	FreeSpec();

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
}