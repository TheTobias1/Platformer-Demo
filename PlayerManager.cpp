#include "PlayerCharacter.h"
#include "PlayerManager.h"
#include "SDL.h"

PlayerCharacter* firstPlayer = nullptr;
PlayerCharacter* secondPlayer = nullptr;

Uint32 firstPlayerRespawnStamp = 0;
Uint32 secondPlayerRespawnStamp = 0;

PlayerCharacter* SpawnPlayer(int posX, int posY, int playerNum)
{
	PlayerCharacter* pl = new PlayerCharacter(posX, posY, playerNum);

	if (playerNum == 0)
	{
		firstPlayer = pl;
		return firstPlayer;
	}
	else
	{
		secondPlayer = pl;
		return secondPlayer;
	}
}

void DespawnPlayer(int playerNum, bool respawn)
{
	Uint32* stamp = nullptr;

	if (playerNum == 0)
	{
		delete firstPlayer;
		firstPlayer = nullptr;

		stamp = &firstPlayerRespawnStamp;
	}
	else
	{
		delete secondPlayer;
		secondPlayer = nullptr;

		stamp = &secondPlayerRespawnStamp;
	}

	if (respawn)
	{
		*stamp = SDL_GetTicks() + 2000;
	}
	else
	{
		*stamp = 0;
	}
}

PlayerCharacter* GetPlayer(int playerNum)
{
	if (playerNum == 0)
	{
		return firstPlayer;
	}
	else
	{
		return secondPlayer;
	}
}

void CheckRespawn()
{
	if (firstPlayer == nullptr && firstPlayerRespawnStamp != 0 && SDL_GetTicks() > firstPlayerRespawnStamp)
	{
		SpawnPlayer(20, 500, 0);
	}

	if (secondPlayer == nullptr && secondPlayerRespawnStamp != 0 && SDL_GetTicks() > secondPlayerRespawnStamp)
	{
		SpawnPlayer(1000, 500, 1);
	}
}

int OtherPlayer(int thisPlayer)
{
	if (thisPlayer == 0)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

PlayerCharacter* GetOtherPlayer(int thisPlayer)
{
	return GetPlayer(OtherPlayer(thisPlayer));
}

void FreePlayers()
{
	delete firstPlayer;
	delete secondPlayer;
}