#pragma once
const int MAX_PLAYERS = 2;

class PlayerInput
{
public:
	PlayerInput();
	PlayerInput(int l, int r, int jmp, int atck, int dsh);
	void EvaluateKeyboard(const Uint8* keyboardState);

	int direction = 0;
	bool jump = false;
	bool dash = false;
	bool attack = false;
	int leftKey;
private:

	int rightKey;
	int jumpKey;
	int attackKey;
	int dashKey;
};

int GetMenuDirection();
bool EnterPressed();
bool EscapePressed();
bool QuitPressed();
void InitialiseInput();
void PollInput();
PlayerInput* GetPlayerInput(int player);
void FreeInput();
