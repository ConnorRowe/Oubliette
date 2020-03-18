// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CoreUObject/Public/Uobject/ConstructorHelpers.h"
#include "Engine/Classes/Components/InstancedStaticMeshComponent.h"
#include "Engine/Classes/Components/BoxComponent.h"
#include "OublietteRoom.generated.h"

UCLASS()
class OUBLIETTE_API AOublietteRoom : public AActor
{
	GENERATED_BODY()

public:
	AOublietteRoom(const FObjectInitializer& ObjectInitializer);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	UPROPERTY(EditAnywhere, Category = "Level Generation")
		int32 roomType;

	UPROPERTY(BlueprintReadWrite)
		UStaticMeshComponent* FloorMesh;
	UPROPERTY(BlueprintReadWrite)
		UBoxComponent* RoomBoxVolume;
};
