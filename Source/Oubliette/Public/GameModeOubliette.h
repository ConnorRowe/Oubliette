// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Oubliette.h"
#include "GameFramework/GameModeBase.h"
#include "GameModeOubliette.generated.h"

//~~~ Forward Declarations
class AOublietteCharacter;

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
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Game Mode")
	TArray<FSpellProjectileStruct> Spells_Projectile;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Game Mode")
	TArray<FSpellChannelStruct> Spells_Channel;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Game Mode")
	TArray<FSpellHitScanStruct> Spells_HitScan;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Game Mode")
	TArray<FSpellUtilStruct> Spells_Utility;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game Mode")
	FItemPoolStruct ItemPool_GeneralChest;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game Mode")
	TArray<FItemStruct> ItemPool_Starter;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game Mode")
	TArray<FItemAffixStruct> ItemPrefixes;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game Mode")
	TArray<FItemAffixStruct> ItemSuffixes;

	UFUNCTION(BlueprintCallable, Category = "Items")
	FItemStruct GenerateItemFromPool(const FItemPoolStruct ItemPool);

	AOublietteCharacter* charRef;
};
