#pragma once

#include "PlayerCharacter.h"

PlayerCharacter* SpawnPlayer(int posX, int posY, int playerNum);
void DespawnPlayer(int playerNum, bool respawn);
void CheckRespawn();

PlayerCharacter* GetPlayer(int playerNum);
int OtherPlayer(int thisPlayer);
PlayerCharacter* GetOtherPlayer(int thisPlayer);

void FreePlayers();