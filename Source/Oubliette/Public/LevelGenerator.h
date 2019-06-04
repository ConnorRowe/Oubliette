// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Kismet/GameplayStatics.h"
#include "GameModeOubliette.h"
#include "GameInstanceOubliette.h"
#include "OublietteCharacter.h"
#include "LevelGenerator.generated.h"

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
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALevelGenerator();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	UFUNCTION(BlueprintCallable, Category = "Level Generation")
	void setGenInfo(const int32 XSize, const int32 YSize, const int32 NumRooms, const float RoomSize, const float RoomMargins, UClass* RoomBP, UClass* WallBP, UClass* WallDoorBP, UClass* CharBP);
	TArray<FRoomData> generateRooms();
	TArray<FWallData> generateWalls();
	UFUNCTION(BlueprintCallable, Category = "Level Generation")
	void spawnLevel();

	UPROPERTY(BlueprintReadOnly, Category = "Level Generation")
	TArray<FRoomData> roomData;
	
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
};
