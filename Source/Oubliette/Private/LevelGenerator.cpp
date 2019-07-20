// Fill out your copyright notice in the Description page of Project Settings.

#include "LevelGenerator.h"
#include "CoreMinimal.h"

// Sets default values
ALevelGenerator::ALevelGenerator(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	UWorld* const w = GetWorld();

	// Create ISM component
	WallMeshInstances = ObjectInitializer.CreateDefaultSubobject<UInstancedStaticMeshComponent>(this, TEXT("WallMeshInstances"));

	// Must set root component;
	RootComponent = WallMeshInstances;

	// Room object generation stuff
	// rooms are 3200^2 units in size
	TArray<FObjectDataStruct> tempRoomData;

	/// Standard Rooms

	// two enemies, 1 table
	tempRoomData.Add(FObjectDataStruct(EObjectTypeEnum::OTE_Enemy_Ranged, FVector(-600.0f, 300.0f, 0.0f), FRotator()));
	tempRoomData.Add(FObjectDataStruct(EObjectTypeEnum::OTE_Enemy_Standard, FVector(-600.0f, -300.0f, 0.0f), FRotator()));
	tempRoomData.Add(FObjectDataStruct(EObjectTypeEnum::OTE_Table, FVector(600.0f, -600.0f, 0.0f), FRotator(0.0f, 180.0f, 0.0f)));
	RoomSpawns_Standard.Add(FRoomGenDataStruct(1, tempRoomData));
	tempRoomData.Empty();

	// 2 tables
	tempRoomData.Add(FObjectDataStruct(EObjectTypeEnum::OTE_Table, FVector(600.0f, -600.0f, 0.0f), FRotator()));
	tempRoomData.Add(FObjectDataStruct(EObjectTypeEnum::OTE_Table, FVector(-600.0f, 600.0f, 0.0f), FRotator()));
	RoomSpawns_Standard.Add(FRoomGenDataStruct(1, tempRoomData));
	tempRoomData.Empty();

	/// Treasure Rooms

	// single middle chest
	tempRoomData.Add(FObjectDataStruct(EObjectTypeEnum::OTE_Chest, FVector(0.0f), FRotator()));
	RoomSpawns_Treasure.Add(FRoomGenDataStruct(2, tempRoomData));
	tempRoomData.Empty();

	// double chest
	tempRoomData.Add(FObjectDataStruct(EObjectTypeEnum::OTE_Chest, FVector(0.0f, 400.0f, 0.0f), FRotator()));
	tempRoomData.Add(FObjectDataStruct(EObjectTypeEnum::OTE_Chest, FVector(0.0f, -400.0f, 0.0f), FRotator()));
	RoomSpawns_Treasure.Add(FRoomGenDataStruct(2, tempRoomData));
	tempRoomData.Empty();

	/// Boss Rooms

	// giant slime
	tempRoomData.Add(FObjectDataStruct(EObjectTypeEnum::OTE_Enemy_Large, FVector(0.0f), FRotator()));
	RoomSpawns_Boss.Add(FRoomGenDataStruct(3, tempRoomData));
	tempRoomData.Empty();

	/// Asset Loading
	// Mesh loading
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> TableDMeshObj(TEXT("/Game/Meshes/Static/Destructible/SM_Table_DM.SM_Table_DM"));
	TableDMesh = Cast<UDestructibleMesh>(TableDMeshObj.Object);

	//Prevents unhandled exception crash (probably from UE-71147)
	if (GetWorld())
	{
		BP_Slime = LoadBPFromPath(TEXT("Blueprint'/Game/Blueprint/AI/Enemies/BP_Enemy_Slime.BP_Enemy_Slime'"));
		BP_Chest = LoadBPFromPath(TEXT("Blueprint'/Game/Blueprint/Items/BP_Chest.BP_Chest'"));
		BP_Slime_Fire = LoadBPFromPath(TEXT("Blueprint'/Game/Blueprint/AI/Enemies/BP_Enemy_Slime_Fire.BP_Enemy_Slime_Fire'"));
		BP_Slime_Giant = LoadBPFromPath(TEXT("Blueprint'/Game/Blueprint/AI/Enemies/BP_Enemy_Slime_Giant.BP_Enemy_Slime_Giant'"));
	}
}

void ALevelGenerator::GenerateObjects(AOublietteRoom* targetRoom)
{
	UWorld* const w = GetWorld();
	TArray<FRoomGenDataStruct> RoomSpawns;

	switch (targetRoom->roomType)
	{
	case 1:
		// Standard room
		RoomSpawns = RoomSpawns_Standard;
		break;
	case 2:
		// Treasure room
		RoomSpawns = RoomSpawns_Treasure;
		break;
	case 3:
		// Boss room
		RoomSpawns = RoomSpawns_Boss;
		break;
	default:
		break;
	}

	int32 maxSize = RoomSpawns.Num() - 1;
	int32 roomindex = FMath::RandRange(0, maxSize);
	TArray<FObjectDataStruct> RoomData = RoomSpawns[roomindex].objects;


	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	for (int i = 0; i < RoomData.Num(); ++i)
	{
		switch (RoomData[i].ObjectType)
		{

		case EObjectTypeEnum::OTE_Table:
		{
			UDestructibleComponent* TableComp = NewObject<UDestructibleComponent>(targetRoom, UDestructibleComponent::StaticClass(), FName("TableDestructibleMesh" + i));
			TableComp->RegisterComponent();
			TableComp->SetDestructibleMesh(TableDMesh);
			TableComp->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
			TableComp->SetSimulatePhysics(true);
			TableComp->SetCanEverAffectNavigation(true);
			TableComp->SetWorldLocationAndRotation((targetRoom->GetActorLocation() + RoomData[i].Location), RoomData[i].Rotation);
			TableComp->SetWorldScale3D(FVector(1.0f));

			break;
		}
		case EObjectTypeEnum::OTE_Chest:
		{
			w->SpawnActor<AActor>(BP_Chest, (targetRoom->GetActorLocation() + RoomData[i].Location), RoomData[i].Rotation, SpawnParams);

			break;
		}
		case EObjectTypeEnum::OTE_Enemy_Standard:
		{
			w->SpawnActor<AActor>(BP_Slime, (targetRoom->GetActorLocation() + RoomData[i].Location), RoomData[i].Rotation, SpawnParams);

			break;
		}
		case EObjectTypeEnum::OTE_Enemy_Ranged:
		{
			w->SpawnActor<AActor>(BP_Slime_Fire, (targetRoom->GetActorLocation() + RoomData[i].Location), RoomData[i].Rotation, SpawnParams);

			break;
		}
		case EObjectTypeEnum::OTE_Enemy_Large:
		{
			w->SpawnActor<AActor>(BP_Slime_Giant, (targetRoom->GetActorLocation() + RoomData[i].Location), RoomData[i].Rotation, SpawnParams);

			break;
		}
		default:
			break;
		}
	}
}

// Called when the game starts or when spawned
void ALevelGenerator::BeginPlay()
{
	Super::BeginPlay();

	WallMeshInstances->SetStaticMesh(wallMesh);

}

void ALevelGenerator::spawnRegWall(const FVector Location, const float ZRot)
{
	// Set up transform for the wall
	FTransform wallTransform;
	wallTransform.SetLocation(Location);
	wallTransform.SetRotation(FQuat(FRotator(0.0f, ZRot, 0.0f)));

	// Spawn wall instance
	WallMeshInstances->AddInstance(wallTransform);
}

TArray<FRoomData> ALevelGenerator::generateRooms()
{
	roomData.Empty();

	int32 treasureRooms = FMath::RandRange(1, 2);
	int32 bossRooms = 1;

	TArray<int32> requiredRooms;

	//Add treasure rooms
	for (int i = 0; i < treasureRooms; ++i)
	{
		requiredRooms.Add(2);
	}
	//Add boss rooms
	for (int i = 0; i < bossRooms; ++i)
	{
		requiredRooms.Add(3);
	}

	//Parse array, setting all room IDs to 0 (empty)
	for (int x = 0; x < xSize; ++x)
	{
		for (int y = 0; y < ySize; ++y)
		{
			roomIDs[x][y] = 0;
		}
	}

	//Pick random starting point in array
	int xPos = FMath::RandRange(0, xSize);
	int yPos = FMath::RandRange(0, ySize);

	//Directions
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
			int newDir = FMath::RandRange(0,3);
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
			if (roomIDs[xPos + xD][yPos + yD] == 0 && (xPos + xD) < xSize && (yPos + yD) < ySize && (xPos + xD) >= 0 && (yPos + yD) >= 0)
			{
				xPos += xD; yPos += yD;

				//Set room ID
				roomIDs[xPos][yPos] = 1;

				roomOK = true;
				tries = 0;
			}

			if (tries > 1000)
			{
				//AKA somethings wrong and should probably skip to avoid crash
				roomOK = true;
				tries = 0;
			}
		}
	}

	//Parse the roomIDs array one final time and convert to a TArray that blueprints understand then return it
	FRoomData tempData;
	int r = 0;
	for (int x = 0; x < xSize; ++x)
	{
		for (int y = 0; y < ySize; ++y)
		{
			if (roomIDs[x][y] == 1)
			{
				//Don't let the first room be special - Sorry, first room!
				if (r > 0)
				{
					//Make sure that there are enough rooms left for normal rooms to be generated
					if (numRooms - r < (requiredRooms.Num() - 1))
					{
						tempData.roomType = requiredRooms.Pop(true);
					}// Just make a room special randomly sometimes to spread them out through the level instead of leaving until last;
					else if (FMath::RandBool() && (requiredRooms.Num() > 0))
					{
						tempData.roomType = requiredRooms.Pop(true);
					}// Otherwise make it a normal room
					else
					{
						tempData.roomType = 1;
					}
				}
				else
				{
					tempData.roomType = 1;
				}

				tempData.xPos = x;
				tempData.yPos = y;

				roomData.Emplace(tempData);
				++r;
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
			if (roomIDs[x][y] > 0)
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
					yWallIDs[x + 1][y] = 1;
					yWallRots[x + 1][y] = 90;
				}

				//There is a room to the right - needs a door
				if (roomIDs[x + 1][y] != 0)
				{
					xWallIDs[x + 1][y] = 2;
					xWallRots[x + 1][y] = 0;
				}


				//There is NOT a room to the right - needs a normal wall
				if (roomIDs[x + 1][y] == 0)
				{
					xWallIDs[x + 1][y] = 1;
					xWallRots[x + 1][y] = 0;
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
					yWallIDs[x][y + 1] = 2;
					yWallRots[x][y + 1] = 90;
				}
				//There is NOT a room to the bottom - needs a normal wall
				if (roomIDs[x][y + 1] == 0)
				{
					yWallIDs[x][y + 1] = 1;
					yWallRots[x][y + 1] = 90;
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

//From the rooms generated from the generateLevels function, this spawns all the necessary objects into the world
void ALevelGenerator::spawnLevel()
{
	UWorld* const w = GetWorld();

	//Starting location for room
	const float roomStart = roomSize + roomMargins;
	//Reference to the game mode
	AGameModeOubliette* gm = (AGameModeOubliette*)w->GetAuthGameMode();
	//Reference to the game instance
	UGameInstanceOubliette* gi = (UGameInstanceOubliette*)w->GetGameInstance();
	FActorSpawnParameters spawnParams;

	//Generate the room data
	TArray<FRoomData> rooms = generateRooms();

	//Spawn all room actors
	for (int i = 0; i < rooms.Num(); ++i)
	{
		//Calculate a new room location depending on the current rooms x and y positions
		FVector roomLoc = FVector(roomStart * rooms[i].xPos, roomStart * rooms[i].yPos, 0.0f) - FVector((roomStart * xSize) / 2);
		roomLoc.Z = 0.0f;

		//Spawn the room actor and add it to the allRooms array in the game mode
		AOublietteRoom* newRoom = w->SpawnActor<AOublietteRoom>(roomBP, roomLoc, FRotator(0.0f), spawnParams);
		gm->allRooms.Emplace(newRoom);
		newRoom->roomType = rooms[i].roomType;

		//If it is the first room, spawn the character, possess it, and set reference in the game instance
		if (i == 0)
		{
			spawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
			AOublietteCharacter* newChar = w->SpawnActor<AOublietteCharacter>(charBP, roomLoc + FVector(0.0f, 0.0f, 100.0f), FRotator(0.0f), spawnParams);
			APlayerController* conRef = UGameplayStatics::GetPlayerController(w, 0);

			conRef->GetPawn()->Destroy();

			conRef->Possess(newChar);
			gi->charRef = newChar;
		}
		else
		{
			//Only generate stuff if it is not the first room
			GenerateObjects(newRoom);
		}
	}

	//Generate walls from the rooms data
	TArray<FWallData> newWalls = generateWalls();

	//Walls should always be spawned, no matter the collision
	spawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	//Spawn all wall and door actors
	for (int i = 0; i < newWalls.Num(); ++i)
	{
		//If the wall type is 0 then no wall should be spawned in that location
		if (newWalls[i].wallType > 0)
		{
			FVector wallLoc = FVector(roomStart * newWalls[i].xPos, roomStart * newWalls[i].yPos, 0.0f) - FVector((roomStart * xSize) / 2);
			wallLoc.Z = 0;

			if (newWalls[i].zRot > 0)
				wallLoc.Y -= roomStart / 2;
			else
				wallLoc.X -= roomStart / 2;

			if (newWalls[i].wallType == 1)
				spawnRegWall(wallLoc, (float)newWalls[i].zRot);
			else if (newWalls[i].wallType == 2)
				gm->allWallsDoors.Emplace(w->SpawnActor<AActor>(wallDoorBP, wallLoc, FRotator(0.0f, (float)newWalls[i].zRot, 0.0f), spawnParams));

		}
	}
}

//Sets all the variables needed to generate and spawn a level
void ALevelGenerator::setGenInfo(const int32 XSize, const int32 YSize, const int32 NumRooms, const float RoomSize, const float RoomMargins, UClass* RoomBP, UClass* WallBP, UClass* WallDoorBP, UClass* CharBP)
{
	if (XSize >= 50)
	{
		xSize = 50;
	}
	else
		xSize = XSize;

	if (YSize >= 50)
	{
		ySize = 50;
	}
	else
		ySize = YSize;

	if (NumRooms >= 2500)
	{
		numRooms = 2500;
	}
	else
		numRooms = NumRooms;

	roomSize = RoomSize;
	roomMargins = RoomMargins;
	roomBP = RoomBP;
	wallBP = WallBP;
	wallDoorBP = WallDoorBP;
	charBP = CharBP;
}

