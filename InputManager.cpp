#include "SDL.h"
#include "InputManager.h"
#include <iostream>

#pragma region Variables
PlayerInput * firstPlayerInput;
PlayerInput * secondPlayerInput;

//Menu variables
int menuDirection = 0;
bool enterPressed = false;
bool escapePressed = false;
bool quit = false;
Uint32 nextMenuInput = 0;
#pragma endregion

#pragma region Player Input
PlayerInput::PlayerInput()
{
	leftKey = 0;
	rightKey = 0;
	jumpKey = 0;
	attackKey = 0;
	dashKey = 0;
}

PlayerInput::PlayerInput(int l, int r, int jmp, int atck, int dsh)
{
	leftKey = l;
	rightKey = r;
	jumpKey = jmp;
	attackKey = atck;
	dashKey = dsh;
}

void PlayerInput::EvaluateKeyboard(const Uint8* keyboardState)
{
	int d = 0;

	if (keyboardState[leftKey])
	{
		d = -1;
	}
	else if (keyboardState[rightKey])
	{
		d = 1;
	}
	
	direction = d;

	attack = keyboardState[attackKey] != 0;
	jump = keyboardState[jumpKey] != 0;
	dash = keyboardState[dashKey] != 0;
	if (dash)
		std::cout << "Pressed Dash" << std::endl;
}

PlayerInput* GetPlayerInput(int player)
{
	if (player == 0)
		return firstPlayerInput;
	else
		return secondPlayerInput;
}
#pragma endregion

#pragma region Menu Input
int GetMenuDirection()
{
	return menuDirection;
}
bool EnterPressed()
{
	return enterPressed;
}
bool EscapePressed()
{
	return escapePressed;
}

bool QuitPressed()
{
	return quit;
}
#pragma endregion

#pragma region Input Handling
void InitialiseInput() 
{
	firstPlayerInput = new PlayerInput
	(
		(int)SDL_SCANCODE_A, //Left
		(int)SDL_SCANCODE_D, //Right
		(int)SDL_SCANCODE_W, //Jump
		(int)SDL_SCANCODE_1, //Attack
		(int)SDL_SCANCODE_2  //Dash
	);

	secondPlayerInput = new PlayerInput
	(
		(int)SDL_SCANCODE_LEFT, //Left
		(int)SDL_SCANCODE_RIGHT, //Right
		(int)SDL_SCANCODE_UP, //Jump
		(int)SDL_SCANCODE_N, //Attack
		(int)SDL_SCANCODE_M  //Dash
	);

}

void PollInput()
{
	int num = 104;
	const Uint8* keyboardState = SDL_GetKeyboardState(&num);

	#pragma region Menu Inputs

	//Quit event
	SDL_Event e;
	while (SDL_PollEvent(&e) != 0)
	{
		if (e.type == SDL_QUIT)
		{
			std::cout << "Quit pressed" << std::endl;
			quit = true;
		}
	}

	//Key presses
	int menuDir = 0;
	bool esc = false;
	bool enter = false;

	bool menuKeyPressed = false;

	//Poll keyboard for presses
	if (SDL_GetTicks() > nextMenuInput)
	{
		if (keyboardState[SDL_SCANCODE_W] || keyboardState[SDL_SCANCODE_UP])
		{
			menuDir = 1;
			menuKeyPressed = true;
		}

		else if (keyboardState[SDL_SCANCODE_S] || keyboardState[SDL_SCANCODE_DOWN])
		{
			menuDir = -1;
			menuKeyPressed = true;
		}

		if (keyboardState[SDL_SCANCODE_RETURN])
		{
			enter = true;
		}
		else if (keyboardState[SDL_SCANCODE_ESCAPE])
		{
			esc = true;
		}
	}

	//Apply presses
	if (menuKeyPressed)
	{
		nextMenuInput = SDL_GetTicks() + 200;
		menuDirection = menuDir;
		enterPressed = enter;
		escapePressed = esc;
	}

	#pragma endregion

	#pragma region Player Inputs

	for (int i = 0; i < MAX_PLAYERS; ++i)
	{
		PlayerInput * curInput = GetPlayerInput(i);
		curInput->EvaluateKeyboard(keyboardState);
	}

	#pragma endregion
}

void FreeInput()
{
	delete firstPlayerInput;
	delete secondPlayerInput;
}
#pragma endregion