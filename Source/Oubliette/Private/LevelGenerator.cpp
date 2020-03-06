// Fill out your copyright notice in the Description page of Project Settings.

#include "LevelGenerator.h"
#include "CoreMinimal.h"
#include <delaunator.hpp>

#define CELLSIZE 160.0f
#define HALFCELLSIZE 80.0f
#define DOORDEPTH 7.5f

// Sets default values
ALevelGenerator::ALevelGenerator(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// Create ISM component
	WallMeshInstances = ObjectInitializer.CreateDefaultSubobject<UInstancedStaticMeshComponent>(this, TEXT("WallMeshInstances"));

	// Must set root component;
	RootComponent = WallMeshInstances;

	/// Asset Loading
	// Mesh loading
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> TableDMeshObj(TEXT("/Game/Meshes/Static/Destructible/SM_Table_DM.SM_Table_DM"));
	TableDMesh = Cast<UDestructibleMesh>(TableDMeshObj.Object);
	static ConstructorHelpers::FObjectFinder<UStaticMesh> OneByOneByTenObj(TEXT("StaticMesh'/Game/Meshes/Static/SM_1x1x10_Cuboid.SM_1x1x10_Cuboid'"));
	OneByOneByTenMesh = Cast<UStaticMesh>(OneByOneByTenObj.Object);

	// Blueprint loading
	BP_Slime = LoadBPFromPath(TEXT("Blueprint'/Game/Blueprint/AI/Enemies/BP_Enemy_Slime.BP_Enemy_Slime'"));
	BP_Chest = LoadBPFromPath(TEXT("Blueprint'/Game/Blueprint/Items/BP_Chest.BP_Chest'"));
	BP_Slime_Fire = LoadBPFromPath(TEXT("Blueprint'/Game/Blueprint/AI/Enemies/BP_Enemy_Slime_Fire.BP_Enemy_Slime_Fire'"));
	BP_Slime_Giant = LoadBPFromPath(TEXT("Blueprint'/Game/Blueprint/AI/Enemies/BP_Enemy_Slime_Giant.BP_Enemy_Slime_Giant'"));
	BP_Trapdoor = LoadBPFromPath(TEXT("Blueprint'/Game/Blueprint/DungeonGen/BP_Trapdoor.BP_Trapdoor'"));
	BP_Char = LoadBPFromPath(TEXT("Blueprint'/Game/Blueprint/Character/BP_Oubliette_Character.BP_Oubliette_Character'"));
	BP_Enemy_MeleeWalker = LoadBPFromPath(TEXT("Blueprint'/Game/Blueprint/AI/Enemies/BP_Enemy_MeleeWalker.BP_Enemy_MeleeWalker'"));
	BP_Door = LoadBPFromPath(TEXT("Blueprint'/Game/Blueprint/DungeonGen/BP_Door.BP_Door'"));
}

void ALevelGenerator::cleanUpActors()
{
	//Destroy all the level objects
	for (TActorIterator<AOublietteDoor> DoorItr(w); DoorItr; ++DoorItr)
	{
		w->DestroyActor(*DoorItr);
	}
	for (TActorIterator<AOublietteRoom> RoomItr(w); RoomItr; ++RoomItr)
	{
		w->DestroyActor(*RoomItr);
	}
	for (TActorIterator<AOublietteItem> ItemItr(w); ItemItr; ++ItemItr)
	{
		w->DestroyActor(*ItemItr);
	}
	for (TActorIterator<AEnemyOubliette> EnemyItr(w); EnemyItr; ++EnemyItr)
	{
		w->DestroyActor(*EnemyItr);
	}
	for (TActorIterator<AOublietteChest> ChestItr(w); ChestItr; ++ChestItr)
	{
		w->DestroyActor(*ChestItr);
	}
	for (TActorIterator<AOublietteTrapdoor> TrapDItr(w); TrapDItr; ++TrapDItr)
	{
		w->DestroyActor(*TrapDItr);
	}
	for (TActorIterator<AOublietteDoor> DoorItr(w); DoorItr; ++DoorItr)
	{
		w->DestroyActor(*DoorItr);
	}
}

// Called when the game starts or when spawned
void ALevelGenerator::BeginPlay()
{
	//Reference to world
	w = GetWorld();
	//Reference to the game mode
	gm = (AGameModeOubliette*)w->GetAuthGameMode();
	//Reference to the game instance
	gi = (UGameInstanceOubliette*)w->GetGameInstance();
	//Reference to player controller
	conRef = UGameplayStatics::GetPlayerController(w, 0);

	FActorSpawnParameters spawnParams;
	spawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	WallMeshInstances->SetStaticMesh(OneByOneByTenMesh);

	//Spawn character in world
	charRef = w->SpawnActor<AOublietteCharacter>(BP_Char, FVector(0.0f, 0.0f, 100.0f), FRotator(0.0f), spawnParams);

	//Destroy old pawn
	conRef->GetPawn()->Destroy();
	//Possess new pawn
	conRef->Possess(charRef);
	//Set character reference in game instance
	gi->charRef = charRef;
	gi->ApplySkillStatsAndBuffs();
	gm->charRef = charRef;

	Super::BeginPlay();

}

TArray<FVector4> ALevelGenerator::generateBaseRooms()
{
	TArray<FVector4> newRooms;

	// random device class instance, source of 'true' randomness for initializing random seed
	std::random_device rd;

	// Mersenne twister PRNG, initialized with seed from previous random device instance
	std::mt19937 gen(rd());

	float mean = 0.8f;
	float stddev = 0.95f;

	// instance of class std::normal_distribution with specific mean and stddev
	std::normal_distribution<float> d(mean, stddev);

	float sample;
	for (int i = 0; i < 24; ++i)
	{
		// get random number with normal distribution using gen as random source
		sample = d(gen);

		FVector2D point = getRandomPointInEllipse(6400.0f, 480.0f);

		float width = FMath::Lerp(960.0f, 1280.0f, FMath::Abs(d(gen)));

		FVector2D size = FVector2D(FMath::Lerp(width * .75, width * 1.25, FMath::Abs(d(gen))), width);

		newRooms.Add(FVector4(point, size));
	}

	return newRooms;
}

bool ALevelGenerator::checkRoomsCollision(const TArray<FVector4> roomsIn, TArray<FVector4>& roomsOut)
{
	bool hasCollided = false;
	roomsOut = roomsIn;

	for (int i = 0; i < roomsOut.Num(); ++i)
	{
		for (FVector roomB : roomsOut)
		{
			if (AABBVec4(roomsOut[i], roomB))
			{
				//move i away from j
				FVector2D translateVec = FVector2D(roomsOut[i].X - roomB.X, roomsOut[i].Y - roomB.Y);
				translateVec.Normalize();
				translateVec *= 256.0f;
				roomsOut[i].X += translateVec.X;
				roomsOut[i].Y += translateVec.Y;

				if (!translateVec.Equals(FVector2D(0.0f)))
				{
					hasCollided = true;
				}
			}
		}
	}

	return hasCollided;
}

void ALevelGenerator::clampRooms(const TArray<FVector4> roomsIn, TArray<FVector4>& roomsOut)
{
	roomsOut = TArray<FVector4>();

	for (int i = 0; i < roomsIn.Num(); ++i)
	{
		roomsOut.Add(FVector4(FMath::RoundToFloat(roomsIn[i].X / CELLSIZE) * CELLSIZE, FMath::RoundToFloat(roomsIn[i].Y / CELLSIZE) * CELLSIZE, FMath::RoundToFloat(roomsIn[i].Z / CELLSIZE) * CELLSIZE, FMath::RoundToFloat(roomsIn[i].W / CELLSIZE) * CELLSIZE));
	}
}

void ALevelGenerator::findMainRooms(const TArray<FVector4> roomsIn, TArray<int>& mainRoomIDsOut)
{
	float meanArea = 0.0f;

	for (FVector4 room : roomsIn)
	{
		meanArea += (room.Z * room.W);
	}

	meanArea /= roomsIn.Num();

	float mainThreshhold = 1.0f;

	for (int i = 0; i < roomsIn.Num(); ++i)
	{
		if (roomsIn[i].Z * roomsIn[i].W >= meanArea * mainThreshhold)
		{
			mainRoomIDsOut.Add(i);
		}
	}
}

void ALevelGenerator::triangulateRooms(const TArray<FVector4> roomsIn, TArray<FTriEdge>& edgesOut, TArray<FVector2D>& indexTreeOut)
{
	std::vector<double> coords = std::vector<double>();
	TArray<FVector2D> roomVerts = TArray<FVector2D>();
	indexTreeOut = TArray<FVector2D>(); // x = id of vertex, y = id of connection

	for (FVector4 room : roomsIn)
	{
		coords.insert(coords.end(), (double)room.X + (room.Z / 2));
		coords.insert(coords.end(), (double)room.Y + (room.W / 2));

		roomVerts.Add(FVector2D(room.X + (room.Z / 2), room.Y + (room.W / 2)));
	}

	delaunator::Delaunator d(coords);

	for (std::size_t i = 0; i < d.triangles.size(); i += 3) {

		FVector2D pointA = FVector2D(d.coords[2 * d.triangles[i]], d.coords[2 * d.triangles[i] + 1]);
		FVector2D pointB = FVector2D(d.coords[2 * d.triangles[i + 1]], d.coords[2 * d.triangles[i + 1] + 1]);
		FVector2D pointC = FVector2D(d.coords[2 * d.triangles[i + 2]], d.coords[2 * d.triangles[i + 2] + 1]);

		edgesOut.AddUnique(FTriEdge(pointA, pointB));
		edgesOut.AddUnique(FTriEdge(pointB, pointC));
		edgesOut.AddUnique(FTriEdge(pointC, pointA));


		// Get all indices in rooms array and their connections

		int indexA = roomVerts.Find(pointA);
		int indexB = roomVerts.Find(pointB);
		int indexC = roomVerts.Find(pointC);

		indexTreeOut.Add(FVector2D(indexA, indexB));
		indexTreeOut.Add(FVector2D(indexB, indexC));
		indexTreeOut.Add(FVector2D(indexC, indexA));
	}
}

void ALevelGenerator::minimalSpanningTree(const TArray<FVector4> roomsIn, const TArray<FVector2D> indexTreeIn, TArray<FTriEdge>& edgesOut, TArray<FVector2D>& indexTreeOut)
{
	bool hasCompleted = false; //if the function has visited every node

	indexTreeOut = TArray<FVector2D>();
	edgesOut = TArray<FTriEdge>();
	TArray<int> allUniqueNodes = TArray<int>();
	for (FVector2D edge : indexTreeIn)
	{
		allUniqueNodes.AddUnique(edge.X);
	}

	TArray<int> visitedNodes = TArray<int>();
	//Choose any arbitrary node as root node
	visitedNodes.Add(indexTreeIn[FMath::RandRange(0, indexTreeIn.Num() - 1)].X);

	while (!hasCompleted)
	{
		TArray<FVector2D> outgoingEdges = TArray<FVector2D>();

		//Find outgoing edges of all visited nodes
		for (int node : visitedNodes)
		{
			for (FVector2D edge : indexTreeIn)
			{
				if (edge.X == (float)node)
				{
					outgoingEdges.Add(edge);
				}
			}
		}

		FVector2D selectedEdge; bool foundNewEdge = false;
		TArray<float> edgeDistances = TArray<float>();

		//Find the distance of all edges
		for (FVector2D edge : outgoingEdges)
		{
			FVector2D start = FVector2D(roomsIn[edge.X].X + (roomsIn[edge.X].Z / 2), roomsIn[edge.X].Y + (roomsIn[edge.X].W / 2));
			FVector2D end = FVector2D(roomsIn[edge.Y].X + (roomsIn[edge.Y].Z / 2), roomsIn[edge.Y].Y + (roomsIn[edge.Y].W / 2));

			edgeDistances.Add(FVector2D::Distance(start, end));
		}

		float edgeLimit = 0;	//used to add a lower limit to the shortest edge search if the smallest one leads to an already visited node
		//Find the shortest edge that hasn't been visited
		while (!foundNewEdge)
		{
			float shortestEdgeIndex = 0;
			float shortestEdge = std::numeric_limits<float>::max();

			for (int i = 0; i < edgeDistances.Num(); ++i)
			{
				if (edgeDistances[i] < shortestEdge && edgeDistances[i] > edgeLimit)
				{
					shortestEdge = edgeDistances[i];
					shortestEdgeIndex = i;
				}
			}

			//Check if the node the selected edge leads to has already been visited
			selectedEdge = outgoingEdges[shortestEdgeIndex];
			if (!visitedNodes.Contains(FMath::FloorToInt(selectedEdge.Y)))
			{
				foundNewEdge = true;
			}
			else
			{
				edgeLimit = shortestEdge;
			}
		}

		//Now an edge has been successfully found, add it to the indexTreeOut array and set the node id as visited
		visitedNodes.Add(selectedEdge.Y);
		indexTreeOut.Add(selectedEdge);

		//Once all nodes have been visited, stop searching
		if (visitedNodes.Num() == allUniqueNodes.Num())
		{
			hasCompleted = true;
		}
	}

	//build edgesOut from the minimal spanning edge node indices
	for (FVector2D edgeIndices : indexTreeOut)
	{
		FVector2D start = FVector2D(roomsIn[edgeIndices.X].X + (roomsIn[edgeIndices.X].Z / 2), roomsIn[edgeIndices.X].Y + (roomsIn[edgeIndices.X].W / 2));
		FVector2D end = FVector2D(roomsIn[edgeIndices.Y].X + (roomsIn[edgeIndices.Y].Z / 2), roomsIn[edgeIndices.Y].Y + (roomsIn[edgeIndices.Y].W / 2));

		edgesOut.Add(FTriEdge(start, end));
	}
}

void ALevelGenerator::generateCorridors(const TArray<FTriEdge> edgesIn, TArray<FTriEdge>& edgesOut)
{
	edgesOut = TArray<FTriEdge>();

	for (FTriEdge edge : edgesIn)
	{
		if (FMath::IsNearlyEqual(edge.start.X, edge.end.X, 20))
		{
			edgesOut.Add(FTriEdge(edge.start, FVector2D(edge.start.X, edge.end.Y)));
		}
		else if (FMath::IsNearlyEqual(edge.start.Y, edge.end.Y, 20))
		{
			edgesOut.Add(FTriEdge(edge.start, FVector2D(edge.end.X, edge.start.Y)));
		}
		else
		{
			if (edge.start.X - edge.end.X < edge.start.Y - edge.end.Y)
			{
				edgesOut.Add(FTriEdge(edge.start, FVector2D(edge.start.X, edge.end.Y)));
				edgesOut.Add(FTriEdge(FVector2D(edge.start.X, edge.end.Y), edge.end));

			}
			else
			{
				edgesOut.Add(FTriEdge(edge.start, FVector2D(edge.end.X, edge.start.Y)));
				edgesOut.Add(FTriEdge(FVector2D(edge.end.X, edge.start.Y), edge.end));
			}
		}
	}
}

void ALevelGenerator::populateRooms(const TArray<FVector4> roomsIn)
{
	float specialRoomChance;

	//Special rooms: 0 = chest room, 1 = boss room, 2 = starter room
	TArray<int> specialRooms = TArray<int>({ 2, 0, 1 });
	if (FMath::RandBool()) { specialRooms.Add(0); }

	for (int i = 0; i < roomsIn.Num(); ++i)
	{
		//The chance to spawn a special room increases as the number of remaining rooms decreases
		specialRoomChance = (i + specialRooms.Num()) / roomsIn.Num();

		const FVector4* room = &roomsIn[i];

		int xCells = room->Z / CELLSIZE;
		int yCells = room->W / CELLSIZE;

		int maxEnemies = FMath::FloorToInt((xCells * yCells) * 0.08f);

		std::vector< std::vector<bool>> cellGrid(xCells, std::vector<bool>(yCells));
		for (int x = 0; x < xCells; ++x)
		{
			for (int y = 0; y < yCells; ++y)
			{
				cellGrid[x][y] = false;
			}
		}

		int enemiesNum = FMath::RandRange(1 + (maxEnemies / 4), maxEnemies);

		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
		
		int specialRoomType = -1;
		//Try generate special room stuff
		if (FMath::FRand() <= specialRoomChance)
		{
			int specialRoomIndex = FMath::RandRange(0, specialRooms.Num() - 1);

			specialRoomType = specialRooms[specialRoomIndex];

			//Midpoint of the room
			FVector midCell = FVector(room->X + ((xCells / 2) * CELLSIZE) + HALFCELLSIZE, room->Y + ((yCells / 2) * CELLSIZE) + HALFCELLSIZE, 1.0f);

			switch (specialRoomType)
			{
				//Chest room
			case 0:
				w->SpawnActor<AActor>(BP_Chest, midCell, FRotator(0.0f), SpawnParams);
				break;
				//Boss room
			case 1:
				w->SpawnActor<AActor>(BP_Trapdoor, midCell, FRotator(0.0f), SpawnParams);
				w->SpawnActor<AActor>(BP_Slime_Giant, midCell, FRotator(0.0f), SpawnParams);
				break;
				//Starter room
			case 2:
				charRef->SetActorLocation(midCell + FVector(0.0f, 0.0f, 100.0f));
				break;
			default:
				break;
			}

			//Remove the special room from the array so it is only generated once
			specialRooms.RemoveAt(specialRoomIndex);
		}

		//Only spawn enemies if the room is not a starter room
		if (specialRoomType != 2)
		{
			for (int e = 0; e < enemiesNum; ++e)
			{
				//pick random point in the grid
				int randX = FMath::RandRange(0, xCells - 1);
				int randY = FMath::RandRange(0, yCells - 1);

				if (!cellGrid[randX][randY])
				{
					float xPos = room->X + (randX * CELLSIZE) + HALFCELLSIZE;
					float yPos = room->Y + (randY * CELLSIZE) + HALFCELLSIZE;

					w->SpawnActor<AActor>(getRandomEnemyBP(), FVector(xPos, yPos, 20.0f), FRotator(), SpawnParams);
				}

				cellGrid[randX][randY] = true;
			}
		}
	}
}

void ALevelGenerator::generateDoors(const TArray<FVector4> roomsIn, const TArray<FVector2D> indexTreeIn)
{
	TArray<FVector> doorsPos = TArray<FVector>();

	for (FVector2D relationship : indexTreeIn)
	{
		FVector4 roomA = roomsIn[FMath::FloorToInt(relationship.X)];
		FVector4 roomB = roomsIn[FMath::FloorToInt(relationship.Y)];

		FVector2D roomDifference = FVector2D(roomA.X + roomA.Z / 2, roomA.Y + roomA.W / 2) - FVector2D(roomB.X + roomB.Z / 2, roomB.Y + roomB.W / 2);

		float angle = FMath::Atan2(roomDifference.X, roomDifference.Y);
		// 0 = East; 1 = North; 2 = West; 3 = South
		int ABDir = (FMath::RoundToInt(8 * angle / (2 * PI) + 8) % 8) / 2;
		int BADir = (ABDir + 2) % 4;
		
		float doorZRotA = 0.0f;
		float doorZRotB = 0.0f;

		FVector doorPosA = FVector();
		FVector doorPosB = FVector();
		FVector offsetA = FVector();
		FVector offsetB = FVector();

		switch (ABDir)
		{
		case 0:
			doorPosA = FVector(roomA.X + DOORDEPTH, roomA.Y + FMath::FRandRange(CELLSIZE, roomA.W - CELLSIZE), 0.0f);
			doorPosB = FVector(roomB.X + roomB.Z - DOORDEPTH, roomB.Y + FMath::FRandRange(CELLSIZE, roomB.W - CELLSIZE), 0.0f);
			doorZRotA = 0.0f;
			doorZRotB = 180.0f;
			offsetA = FVector(-HALFCELLSIZE, 0.0f, 0.0f);
			offsetB = FVector(HALFCELLSIZE, 0.0f, 0.0f);
			break;
		case 1:
			doorPosA = FVector(roomA.X + FMath::FRandRange(CELLSIZE, roomA.Z - CELLSIZE), roomA.Y + DOORDEPTH, 0.0f);
			doorPosB = FVector(roomB.X + FMath::FRandRange(CELLSIZE, roomB.Z - CELLSIZE), roomB.Y + roomB.W - DOORDEPTH, 0.0f);
			doorZRotA = 90.0f;
			doorZRotB = -90.0f;
			offsetA = FVector(0.0f, -HALFCELLSIZE, 0.0f);
			offsetB = FVector(0.0f, HALFCELLSIZE, 0.0f);
			break;
		case 2:
			doorPosA = FVector(roomA.X + roomA.Z - DOORDEPTH, roomA.Y + FMath::FRandRange(CELLSIZE, roomA.W - CELLSIZE), 0.0f);
			doorPosB = FVector(roomB.X + DOORDEPTH, roomB.Y + FMath::FRandRange(CELLSIZE, roomB.W - CELLSIZE), 0.0f);
			doorZRotA = 180.0f;
			doorZRotB = 0.0f;
			offsetA = FVector(HALFCELLSIZE, 0.0f, 0.0f);
			offsetB = FVector(-HALFCELLSIZE, 0.0f, 0.0f);
			break;
		case 3:
			doorPosA = FVector(roomA.X + FMath::FRandRange(CELLSIZE, roomA.Z - CELLSIZE), roomA.Y + roomA.W - DOORDEPTH, 0.0f);
			doorPosB = FVector(roomB.X + FMath::FRandRange(CELLSIZE, roomB.Z - CELLSIZE), roomB.Y + DOORDEPTH, 0.0f);
			doorZRotA = -90.0f;
			doorZRotB = 90.0f;
			offsetA = FVector(0.0f, HALFCELLSIZE, 0.0f);
			offsetB = FVector(0.0f, -HALFCELLSIZE, 0.0f);
			break;
		}

		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		AOublietteDoor* doorA = w->SpawnActor<AOublietteDoor>(BP_Door, doorPosA, FRotator(0.0f), SpawnParams);
		AOublietteDoor* doorB = w->SpawnActor<AOublietteDoor>(BP_Door, doorPosB, FRotator(0.0f), SpawnParams);

		doorA->linkedDoor = doorB;
		doorB->linkedDoor = doorA;

		doorA->Init(FRotator(0.0f, doorZRotA, 0.0f), offsetA);
		doorB->Init(FRotator(0.0f, doorZRotB, 0.0f), offsetB);

		doorsPos.Append({ doorPosA, doorPosB });
	}
}

void ALevelGenerator::createWallMeshInstances(const TArray<FVector4> roomsIn)
{
	WallMeshInstances->ClearInstances();

	for (FVector4 room : roomsIn)
	{
		//north wall
		WallMeshInstances->AddInstanceWorldSpace(FTransform(FRotator(0.0f), FVector(room.X + (room.Z / 2), room.Y, 200.0f), FVector(room.Z, 1.0f, 40.0f)));
		//south wall
		WallMeshInstances->AddInstanceWorldSpace(FTransform(FRotator(0.0f), FVector(room.X + (room.Z / 2), room.Y + room.W, 200.0f), FVector(room.Z, 1.0f, 40.0f)));
		//east wall
		WallMeshInstances->AddInstanceWorldSpace(FTransform(FRotator(0.0f), FVector(room.X, room.Y + (room.W / 2), 200.0f), FVector(1.0f, room.W, 40.0f)));
		//west wall
		WallMeshInstances->AddInstanceWorldSpace(FTransform(FRotator(0.0f), FVector(room.X + room.Z, room.Y + (room.W / 2), 200.0f), FVector(1.0f, room.W, 40.0f)));
	}
}

bool ALevelGenerator::AABBVec4(const FVector4 roomA, const FVector4 roomB)
{
	const float extraSpace = 720.0f;

	// Collision x-axis?
	bool collisionX = roomA.X + roomA.Z + extraSpace >= roomB.X &&
		roomB.X + roomB.Z + extraSpace >= roomA.X;
	// Collision y-axis?
	bool collisionY = roomA.Y + roomA.W + extraSpace >= roomB.Y &&
		roomB.Y + roomB.W + extraSpace >= roomA.Y;
	// Collision only if on both axes
	return collisionX && collisionY;
}

FVector2D ALevelGenerator::getRandomPointInEllipse(float width, float height)
{
	float t = 2.0f * PI * FMath::FRand();
	float u = FMath::FRand() + FMath::FRand();
	float r;

	if (u > 1.0f)
	{
		r = 2.0f - u;
	}
	else
	{
		r = u;
	}

	return FVector2D(width * r * FMath::Cos(t) / 2.0f, height * r * FMath::Cos(t) / 2.0f);
}

UClass* ALevelGenerator::getRandomEnemyBP()
{
	TArray<UClass*> enemies = TArray<UClass*>({ BP_Slime, BP_Slime_Fire, BP_Enemy_MeleeWalker });

	return enemies[FMath::RandRange(0, enemies.Num() - 1)];
}