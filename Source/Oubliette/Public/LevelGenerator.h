// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
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
	void setGenInfo(int32 xsize, int32 ysize, int32 numrooms);
	UFUNCTION(BlueprintCallable, Category = "Level Generation")
	TArray<FRoomData> generateLevels();


	UPROPERTY(BlueprintReadWrite, Category = "Level Generation")
	int32 xSize;
	UPROPERTY(BlueprintReadWrite, Category = "Level Generation")
	int32 ySize;
	UPROPERTY(BlueprintReadWrite, Category = "Level Generation")
	int32 numRooms;

	UPROPERTY(BlueprintReadOnly, Category = "Level Generation")
	TArray<FRoomData> roomData;
	
private:
	int32 roomIDs[15][15];

};
