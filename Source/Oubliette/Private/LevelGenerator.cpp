// Fill out your copyright notice in the Description page of Project Settings.

#include "CoreMinimal.h"
#include "LevelGenerator.h"


// Sets default values
ALevelGenerator::ALevelGenerator()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ALevelGenerator::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void ALevelGenerator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

TArray<FRoomData> ALevelGenerator::generateLevels()
{
	roomData.Empty();
	
	//Parse array, setting all room IDs to 0 (empty)
	for (int x = 0; x < xSize; ++x)
	{
		for (int y = 0; y < ySize; ++y)
		{
			roomIDs[x][y] = 0;
		}
	}

	//Pick random starting point in array
	int xPos = rand() % xSize;
	int yPos = rand() % ySize;

	int xD, yD = 0;

	int tries = 0;

	for (int r = 1; r < numRooms; ++r)
	{
		bool roomOK = false;
		roomIDs[xPos][yPos] = 1;

		while (!roomOK)
		{
			++tries;
			//Picking a new position for next room
			int newDir = rand() % 4;
			switch (newDir)
			{
			case 0:
				xD = 1; yD = 0;
				break;
			case 1:
				xD = 0; yD = 1;
				break;
			case 2:
				xD = -1; yD = 0;
				break;
			case 3:
				xD = 0; yD = -1;
				break;
			default:
				xD = 0; yD = 0;
				break;
			}

			//Room space exists and is empty
			//if (roomIDs[xPos + xD][yPos + yD] != NULL && roomIDs[xPos + xD][yPos + yD] == 0)
			if (roomIDs[xPos + xD][yPos + yD] == 0 && (xPos+xD) < xSize && (yPos + yD) < ySize && (xPos + xD) >= 0 && (yPos + yD) >= 0)
			{
				xPos += xD; yPos += yD;
				//setting new ID
				roomIDs[xPos][yPos] = 1;
				roomOK = true;
				tries = 0;
			}

			if (tries > 100)
			{
				//AKA somethings wrong and should probably skip to avoid crash
				roomOK = true;
				tries = 0;
			}
		}
	}

	//Parse the roomIDs array one final time and convert to a TArray that blueprints understand then return it
	FRoomData tempData;
	for (int x = 0; x < xSize; ++x)
	{
		for (int y = 0; y < ySize; ++y)
		{
			if (roomIDs[x][y] == 1)
			{
				tempData.roomType = 1;
				tempData.xPos = x;
				tempData.yPos = y;

				roomData.Emplace(tempData);
			}
		}
	}

	return roomData;
}

TArray<FWallData> ALevelGenerator::generateWalls()
{
	int32 xWallIDs[51][51];
	int32 xWallRots[51][51];
	int32 yWallIDs[51][51];
	int32 yWallRots[51][51];

	for (int x = 0; x < xSize; ++x)
	{
		for (int y = 0; y < ySize; ++y)
		{
			//If room exists
			if (roomIDs[x][y] == 1)
			{
				//Room is completely left / right
				if (x == 0 || x == xSize)
				{
					xWallIDs[x][y] = 1;
					xWallRots[x][y] = 0;
				}
				//Room is completly top / bottom
				if (y == 1 || y == ySize)
				{
					yWallIDs[x+1][y] = 1;
					yWallRots[x+1][y] = 90;
				}

				//There is a room to the right - needs a door
				if (roomIDs[x + 1][y] != 0)
				{
					xWallIDs[x+1][y] = 2;
					xWallRots[x+1][y] = 0;
				}


				//There is NOT a room to the right - needs a normal wall
				if (roomIDs[x + 1][y] == 0)
				{
					xWallIDs[x+1][y] = 1;
					xWallRots[x+1][y] = 0;
				}
				//There is NOT a room to the left - needs a normal wall
				if (roomIDs[x - 1][y] == 0)
				{
					xWallIDs[x][y] = 1;
					xWallRots[x][y] = 0;
				}

				//There is a room to the bottom - needs a door
				if (roomIDs[x][y + 1] != 0)
				{
					yWallIDs[x][y+1] = 2;
					yWallRots[x][y+1] = 90;
				}
				//There is NOT a room to the bottom - needs a normal wall
				if (roomIDs[x][y + 1] == 0)
				{
					yWallIDs[x][y+1] = 1;
					yWallRots[x][y+1] = 90;
				}
				//There is NOT a room to the top - needs a normal wall
				if (roomIDs[x][y - 1] == 0)
				{
					yWallIDs[x][y] = 1;
					yWallRots[x][y] = 90;
				}
			}
		}
	}

	TArray<FWallData> wallData;
	FWallData tempData;

	//Parse the wallIDs array to and convert to a TArray that blueprints understand then return it
	for (int x = 0; x < xSize + 1; ++x)
	{
		for (int y = 0; y < ySize + 1; ++y)
		{
			if (xWallIDs[x][y] == 1 || xWallIDs[x][y] == 2)
			{
				tempData.wallType = xWallIDs[x][y];
				tempData.xPos = x;
				tempData.yPos = y;
				tempData.zRot = xWallRots[x][y];

				wallData.Emplace(tempData);
			}

			if (yWallIDs[x][y] == 1 || yWallIDs[x][y] == 2)
			{
				tempData.wallType = yWallIDs[x][y];
				tempData.xPos = x;
				tempData.yPos = y;
				tempData.zRot = yWallRots[x][y];

				wallData.Emplace(tempData);
			}
		}
	}
	
	return wallData;
}

void ALevelGenerator::setGenInfo(int32 xsize, int32 ysize, int32 numrooms)
{
	if (xsize >= 50)
	{
		xSize = 50;
	}
	else
		xSize = xsize;

	if (ysize >= 50)
	{
		ySize = 50;
	}
	else
		ySize = ysize;

	if (numRooms >= 2500)
	{
		numRooms = 2500;
	}
	else
		numRooms = numrooms;
}

