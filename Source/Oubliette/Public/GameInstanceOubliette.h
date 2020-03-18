// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "OublietteCharacter.h"
#include "Oubliette.h"
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
	UPROPERTY(BlueprintReadWrite, Category = "Game Instance")
		bool isGameOver;
	UPROPERTY(BlueprintReadWrite, Category = "Character")
		TArray<FStatStruct> SkillStats;
	UPROPERTY(BlueprintReadWrite, Category = "Character")
		TArray<FBuffStruct> SkillBuffs;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game Stats")
		int32 Track_EnemiesKilled;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game Stats")
		int32 Track_FloorsComplete;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game Stats")
		int32 Track_ItemsFound;

	void ApplySkillStatsAndBuffs();

	UFUNCTION(BlueprintCallable, Category = "Game Stats")
		void ClearTrackedVars();
};
