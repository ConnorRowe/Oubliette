// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "GameModeOubliette.generated.h"

/**
 * 
 */
UCLASS()
class OUBLIETTE_API AGameModeOubliette : public AGameModeBase
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, Category = "Game Mode")
	TArray<AActor*> allRooms;
	
};
