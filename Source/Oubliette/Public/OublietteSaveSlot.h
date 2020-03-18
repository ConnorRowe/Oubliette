// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "OublietteSaveSlot.generated.h"

/**
 *
 */
UCLASS()
class OUBLIETTE_API UOublietteSaveSlot : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lineage Saveslot")
		int32 SaveID;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lineage Saveslot")
		int32 MetaID;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lineage Saveslot")
		FString LineageName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lineage Saveslot")
		int32 LineageLevel;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lineage Saveslot")
		FDateTime DTLastPlayed;

	UFUNCTION(BlueprintCallable, Category = "Lineage Saveslot")
		void init(const int32 newSaveID, const int32 newMetaID, const FString newLineageName, const int32 newLineageLevel, const FDateTime newDTLastPlayed);
};
