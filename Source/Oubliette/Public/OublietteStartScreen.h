// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Oubliette.h"
#include "OublietteSaveSlot.h"
#include "Algo/Reverse.h"
#include "Blueprint/UserWidget.h"
#include "OublietteStartScreen.generated.h"

/**
 *
 */
UCLASS()
class OUBLIETTE_API UOublietteStartScreen : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Saves")
		TArray<FSaveSlotStruct> SortSaves(TArray<FSaveSlotStruct> SaveSlots, ESortMethodEnum SortMethod, bool isInverse);

};
