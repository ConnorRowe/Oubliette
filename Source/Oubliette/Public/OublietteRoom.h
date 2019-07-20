// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/Classes/Components/InstancedStaticMeshComponent.h"
#include "OublietteRoom.generated.h"

UCLASS()
class OUBLIETTE_API AOublietteRoom : public AActor
{
	GENERATED_UCLASS_BODY()

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	UPROPERTY(EditAnywhere, Category = "Level Generation")
	int32 roomType;
};
