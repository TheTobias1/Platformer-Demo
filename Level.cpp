#include <vector>
#include "Body.h"
#include "Level.h"
#include "SDL.h"
#include "SDL_image.h"
#include <iostream>
#include <fstream>
#include <istream>
#include <sstream>
#include <string>
#include <iostream>
using namespace std;

#define TILE_FREE 0
#define TILE_OCCUPIED 1
#define TILE_PARSED 2

vector<Body*> levelPlatforms;
int numPlatforms = 5;

SDL_Texture* levelImage;

int collisionMatrix[34][23] = { 0 };

pair<int, int> MatrixToPixelTransformation(int x, int y)
{
	pair<int, int> coord;

	coord.first = x * 32;
	coord.second = y * 32;

	return coord;
}

void InitialiseLevel(SDL_Renderer* rend)
{
	//Image
	SDL_Surface* levelSurface = IMG_Load("Maps/Map1/Map1.png");

	if (levelSurface == NULL)
		std::cout << "NO IMAGE FOUND" << std::endl;

	levelImage = SDL_CreateTextureFromSurface(rend, levelSurface);
	SDL_FreeSurface(levelSurface);

	//Parse CSV
	#pragma region CSV Parsing
	ifstream filePointer;

	filePointer.open("Maps/Map1/Map1_Tile.csv");

	string temp, line;
	char entry;
	int curX = 0;
	int curY = 0;
	bool writeToMatrix = true;

	while (getline(filePointer, line))
	{
		stringstream ss(line);

		while (ss >> entry)
		{
			if (writeToMatrix)
			{
				if (entry == '-')
				{
					collisionMatrix[curX][curY] = TILE_FREE;
					cout << "0";
				}
				else
				{
					collisionMatrix[curX][curY] = TILE_OCCUPIED;
					cout << "1";
				}
				++curX;
				writeToMatrix = false;
			}
			else
			{
				if (entry == ',')
				{
					writeToMatrix = true;
				}
			}
		}

		cout << endl;
		curX = 0;
		curY++;
		writeToMatrix = true;
	}
	#pragma endregion

	cout << "Parsed csv" << endl;
	//Spawning Platforms
	#pragma region Collision Generation

	int platformCount = 0;

	//Iterate over cells in the matrix
	for (curY = 0; curY < 23; ++curY)
	{
		for (curX = 0; curX < 34; ++curX)
		{
			if (collisionMatrix[curX][curY] == TILE_OCCUPIED)
			{
				int rectW = 1;
				int rectH = 1;

				//Define the width
				for (int i = curX + 1; i < 34; ++i)
				{
					if (collisionMatrix[i][curY] == TILE_OCCUPIED)
					{
						collisionMatrix[i][curY] = TILE_PARSED;
						++rectW;
					}
					else
					{
						break;
					}
				}

				//Define the height
				bool validRect = true;

				for (int i = curY + 1; i < 23 && validRect; ++i)
				{
					for (int j = 0; j < rectW; ++j)
					{
						if (collisionMatrix[j + curX][i] != TILE_OCCUPIED)
						{
							validRect = false;
						}
					}

					if (validRect)
					{
						for (int j = 0; j < rectW; ++j)
						{
							collisionMatrix[j + curX][i] = TILE_PARSED;
						}
						++rectH;
					}
				}

				//Spawn platform
				pair<int, int> platformPos = MatrixToPixelTransformation(curX, curY);
				pair<int, int> platformScale = MatrixToPixelTransformation(rectW, rectH);

				if (platformPos.first == 0)
				{
					platformPos.first = -32;
					platformScale.first += 32;
				}

				if (platformPos.first + platformScale.first >= 1088)
				{
					platformScale.first += 32;
				}

				levelPlatforms.push_back(new Body(platformPos.first, platformPos.second, platformScale.first, platformScale.second));

				++platformCount;
				
			}
		}
	}

	numPlatforms = platformCount;

	#pragma endregion



}
vector<Body*> GetLevelPlatforms()
{
	return levelPlatforms;
}
int GetNumPlatforms()
{
	return numPlatforms;
}

void DrawLevel(SDL_Renderer* rend)
{
	SDL_RenderCopy(rend, levelImage, NULL, NULL);
}

void UnloadLevel()
{
	SDL_DestroyTexture(levelImage);
	for (int i = 0; i < levelPlatforms.size(); ++i) 
	{
		delete levelPlatforms[i];
	}

	levelPlatforms.clear();
}