// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EngineMinimal.h"
#include "GameFramework/Actor.h"
#include "Kismet/GameplayStatics.h"
#include "GameModeOubliette.h"
#include "Engine/Classes/Components/InstancedStaticMeshComponent.h"
#include "GameInstanceOubliette.h"
#include "OublietteCharacter.h"
#include "OublietteRoom.h"
#include "OublietteItem.h"
#include "OublietteChest.h"
#include "OublietteDoor.h"
#include "OublietteTrapdoor.h"
#include "EnemyOubliette.h"
#include "DestructibleMesh.h"
#include "ApexDestruction/Public/DestructibleComponent.h"
#include "Engine/World.h"
#include "CoreUObject/Public/Uobject/ConstructorHelpers.h"
#include "EngineUtils.h"
#include <EngineGlobals.h>
#include <Runtime/Engine/Classes/Engine/Engine.h>
#include <random>
#include "OublietteDoor.h"
#include "LevelGenerator.generated.h"

// Enum of different spawnable objects for rooms
enum class EObjectTypeEnum : uint8
{
	OTE_Table,
	OTE_Enemy_Standard,
	OTE_Enemy_Ranged,
	OTE_Enemy_Large,
	OTE_Chest,
	OTE_Trapdoor
};

//Represents a start and end point (x, y)
USTRUCT(BluePrintType)
struct FTriEdge
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(BlueprintReadWrite)
	FVector2D start;
	UPROPERTY(BlueprintReadWrite)
	FVector2D end;

	//override '==' operator so that TArray.AddUnique() works.
	FORCEINLINE bool operator==(const FTriEdge& Other) const
	{
		return (start == Other.start && end == Other.end);
	}

	FTriEdge()
	{
		start = FVector2D(0.0f); end = FVector2D(0.0f);
	}

	FTriEdge(FVector2D Start, FVector2D End)
	{
		start = Start; end = End;
	}
};

UCLASS()
class OUBLIETTE_API ALevelGenerator : public AActor
{
	GENERATED_UCLASS_BODY()

	UPROPERTY(BlueprintReadWrite, Category = "Level Generation")
	class UInstancedStaticMeshComponent* WallMeshInstances;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	AOublietteCharacter* charRef;

	//generates 24 randomly sized and positioned rooms stored as FVector4 (x pos, y pos, width, height)
	UFUNCTION(BlueprintCallable, Category = "Level Generation")
	TArray<FVector4> generateBaseRooms();

	//checks all given rooms for collisions, if one occurs then the room is moved away from the room it is colliding with - returns false if no collisions occur
	UFUNCTION(BlueprintCallable, Category = "Level Generation")
	bool checkRoomsCollision(const TArray<FVector4> roomsIn, TArray<FVector4>& roomsOut);

	//clamps room positions and sizes to the grid cell size
	UFUNCTION(BlueprintCallable, Category = "Level Generation")
	void clampRooms(const TArray<FVector4> roomsIn, TArray<FVector4>& roomsOut);

	//filters rooms to be of a similar size within a range
	UFUNCTION(BlueprintCallable, Category = "Level Generation")
	void findMainRooms(const TArray<FVector4> roomsIn, TArray<int>& mainRoomIDsOut);

	//performs Delaunay triangulation using Delaunator by delfrrr which can be found at https://github.com/delfrrr/delaunator-cpp
	UFUNCTION(BlueprintCallable, Category = "Level Generation")
	void triangulateRooms(const TArray<FVector4> roomsIn, TArray<FTriEdge>& edgesOut, TArray<FVector2D>& indexTreeOut);

	//generates a minimal spanning tree using Prim's algorithm
	UFUNCTION(BlueprintCallable, Category = "Level Generation")
	void minimalSpanningTree(const TArray<FVector4> roomsIn, const TArray<FVector2D> indexTreeIn, TArray<FTriEdge>& edgesOut, TArray<FVector2D>& indexTreeOut);

	//generates corridors between rooms using L corners if needed - unused for gameplay currently
	UFUNCTION(BlueprintCallable, Category = "Level Generation")
	void generateCorridors(const TArray<FTriEdge> edgesIn, TArray<FTriEdge>& edgesOut);

	//spawns objects like enemies and chests into the rooms
	UFUNCTION(BlueprintCallable, Category = "Level Generation")
	void populateRooms(const TArray<FVector4> roomsIn, TArray<ERoomTypeEnum>& roomTypesOut);

	//spawns doors to connect all the rooms using the minimal spanning tree
	UFUNCTION(BlueprintCallable, Category = "Level Generation")
	void generateDoors(const TArray<FVector4> roomsIn, const TArray<FVector2D> indexTreeIn, const TArray<ERoomTypeEnum> roomTypesIn);

	//adds a static mesh instance for each room's 4 walls
	UFUNCTION(BlueprintCallable, Category = "Level Generation")
	void createWallMeshInstances(const TArray<FVector4> roomsIn);

	//iterates through all actors used for level generation and deletes them all
	UFUNCTION(BlueprintCallable, Category = "Level Generation")
	void cleanUpActors();

private:
	UWorld* w;
	AGameModeOubliette* gm;
	UGameInstanceOubliette* gi;
	APlayerController* conRef;

	UClass* BP_Char;

	//Room object stuff
	UDestructibleMesh* TableDMesh;
	UStaticMesh* OneByOneByTenMesh;
	UClass* BP_Chest;
	UClass* BP_Slime;
	UClass* BP_Slime_Fire;
	UClass* BP_Slime_Giant;
	UClass* BP_Enemy_MeleeWalker;
	UClass* BP_Trapdoor;
	UClass* BP_Door;

	//Helper functions

	bool AABBVec4(const FVector4 roomA, const FVector4 roomB);
	FVector2D getRandomPointInEllipse(float width, float height);
	UClass* getRandomEnemyBP();
};
