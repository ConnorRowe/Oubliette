// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "OublietteCharacter.h"
#include "Engine/GameInstance.h"
#include "GameInstanceOubliette.generated.h"

/**
 * 
 */
UCLASS()
class OUBLIETTE_API UGameInstanceOubliette : public UGameInstance
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadWrite, Category = "Game Instance")
	AOublietteCharacter* charRef;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game Stats")
	int32 Track_EnemiesKilled;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game Stats")
	int32 Track_FloorsComplete;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game Stats")
	int32 Track_ItemsFound;
};
