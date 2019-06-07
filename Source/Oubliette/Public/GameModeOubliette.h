// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Oubliette.h"
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
	UPROPERTY(BlueprintReadWrite, Category = "Game Mode")
	TArray<AActor*> allWallsDoors;
	UPROPERTY(BlueprintReadWrite, Category = "Game Mode")
	TArray<FSpellProjectileStructNEW> Spells_Projectile;
	UPROPERTY(BlueprintReadWrite, Category = "Game Mode")
	TArray<FSpellChannelStructNEW> Spells_Channel;
	UPROPERTY(BlueprintReadWrite, Category = "Game Mode")
	TArray<FSpellHitScanStructNEW> Spells_HitScan;
	UPROPERTY(BlueprintReadWrite, Category = "Game Mode")
	TArray<FSpellUtilStructNEW> Spells_Utility;
};
