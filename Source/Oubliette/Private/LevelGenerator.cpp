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

void ALevelGenerator::setGenInfo(int32 xsize, int32 ysize, int32 numrooms)
{
	xSize = xsize;
	ySize = ysize;
	numRooms = numrooms;
}

