// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Kismet/GameplayStatics.h"
#include "GameModeOubliette.h"
#include "Engine/Classes/Components/InstancedStaticMeshComponent.h"
#include "GameInstanceOubliette.h"
#include "OublietteCharacter.h"
#include "LevelGenerator.generated.h"

// Enum of different spawnable objects for rooms
enum class EObjectTypeEnum : uint8
{
	OTE_Table,
	OTE_Enemy_Standard,
	OTE_Enemy_Ranged,
	OTE_Enemy_Large,
	OTE_Chest
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

struct RoomGenData
{
	int32 roomType;
	TArray<FObjectDataStruct> objects;
};

USTRUCT(BlueprintType)
struct FRoomData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(BlueprintReadWrite, Category = "Room Data")
	int32 xPos;
	UPROPERTY(BlueprintReadWrite, Category = "Room Data")
	int32 yPos;
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
	void setGenInfo(const int32 XSize, const int32 YSize, const int32 NumRooms, const float RoomSize, const float RoomMargins, UClass* RoomBP, UClass* WallBP, UClass* WallDoorBP, UClass* CharBP);
	TArray<FRoomData> generateRooms();
	TArray<FWallData> generateWalls();
	UFUNCTION(BlueprintCallable, Category = "Level Generation")
	void spawnLevel();

	UPROPERTY(BlueprintReadOnly, Category = "Level Generation")
	TArray<FRoomData> roomData;

	UPROPERTY(EditDefaultsOnly, Category = "Room")
	UStaticMesh* wallMesh;

	void spawnRegWall(const FVector Location, const float ZRot);

private:
	int32 roomIDs[50][50];
	int32 xSize;
	int32 ySize;
	int32 numRooms;
	int32 roomSize;
	int32 roomMargins;
	UClass* roomBP;
	UClass* wallBP;
	UClass* wallDoorBP;
	UClass* charBP;

	//Room object stuff
	TArray<RoomGenData> RoomSpawns_Standard;
	void GenerateObjects(AActor* targetRoom);
	UStaticMesh* TableMesh;
	UClass* BP_Chest;
	UClass* BP_Slime;
	UClass* BP_Slime_Fire;
	UClass* BP_Slime_Giant;

};
