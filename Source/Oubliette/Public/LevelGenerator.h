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

struct FObjectDataStruct
{
	EObjectTypeEnum ObjectType;
	FVector Location;
	FRotator Rotation;

	FObjectDataStruct() {};

	//Constructor
	FObjectDataStruct(EObjectTypeEnum init_ObjectType, FVector init_Location, FRotator init_Rotation)
	{
		ObjectType = init_ObjectType;
		Location = init_Location;
		Rotation = init_Rotation;
	}
};

struct FRoomGenDataStruct
{
	int32 roomType;
	TArray<FObjectDataStruct> objects;

	FRoomGenDataStruct() {};

	//Constructor
	FRoomGenDataStruct(int32 init_roomType, TArray<FObjectDataStruct> init_objects)
	{
		roomType = init_roomType;
		objects = init_objects;
	}
};

USTRUCT(BlueprintType)
struct FRoomData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(BlueprintReadWrite, Category = "Room Data")
	int32 xPos = NULL;
	UPROPERTY(BlueprintReadWrite, Category = "Room Data")
	int32 yPos = NULL;
	UPROPERTY(BlueprintReadWrite, Category = "Room Data")
	int32 roomType = 0;
};

USTRUCT(BluePrintType)
struct FWallData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(BlueprintReadWrite, Category = "Wall Data")
	int32 xPos = 0;
	UPROPERTY(BlueprintReadWrite, Category = "Wall Data")
	int32 yPos = 0;
	UPROPERTY(BlueprintReadWrite, Category = "Wall Data")
	int32 wallType = 0;
	UPROPERTY(BlueprintReadWrite, Category = "Wall Data")
	int32 zRot = 0;
};

UCLASS()
class OUBLIETTE_API ALevelGenerator : public AActor
{
	GENERATED_UCLASS_BODY()

	UPROPERTY(EditAnywhere, Category = "Level Generation")
	class UInstancedStaticMeshComponent* WallMeshInstances;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	UFUNCTION(BlueprintCallable, Category = "Level Generation")
	void setGenInfo(const int32 XSize, const int32 YSize, const int32 NumRooms, const float RoomSize, const float RoomMargins, UClass* RoomBP, UClass* WallDoorBP);
	TArray<FRoomData> generateRooms();
	TArray<FWallData> generateWalls();
	UFUNCTION(BlueprintCallable, Category = "Level Generation")
	void spawnLevel();
	UFUNCTION(BlueprintCallable, Category = "Level Generation")
	void nextLevel();

	UPROPERTY(BlueprintReadOnly, Category = "Level Generation")
	TArray<FRoomData> roomData;

	UPROPERTY(EditDefaultsOnly, Category = "Room")
	UStaticMesh* wallMesh;

	void spawnRegWall(const FVector Location, const float ZRot);

	AOublietteCharacter* charRef;


private:
	UWorld* w;
	AGameModeOubliette* gm;
	UGameInstanceOubliette* gi;
	APlayerController* conRef;

	int32 roomIDs[50][50];
	int32 xSize;
	int32 ySize;
	int32 numRooms;
	int32 roomSize;
	int32 roomMargins;
	UClass* roomBP;
	UClass* wallDoorBP;
	UClass* BP_Char;

	//Room object stuff
	TArray<FRoomGenDataStruct> RoomSpawns_Standard;
	TArray<FRoomGenDataStruct> RoomSpawns_Treasure;
	TArray<FRoomGenDataStruct> RoomSpawns_Boss;
	void GenerateObjects(AOublietteRoom* targetRoom);
	UDestructibleMesh* TableDMesh;
	UClass* BP_Chest;
	UClass* BP_Slime;
	UClass* BP_Slime_Fire;
	UClass* BP_Slime_Giant;
	UClass* BP_Enemy_MeleeWalker;
	UClass* BP_Trapdoor;

};
