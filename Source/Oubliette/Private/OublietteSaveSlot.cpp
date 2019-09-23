// Fill out your copyright notice in the Description page of Project Settings.


#include "OublietteSaveSlot.h"

void UOublietteSaveSlot::init(const int32 newSaveID, const int32 newMetaID, const FString newLineageName, const int32 newLineageLevel, const FDateTime newDTLastPlayed)
{
	SaveID = newSaveID;
	MetaID = newMetaID;
	LineageName = newLineageName;
	LineageLevel = newLineageLevel;
	DTLastPlayed = newDTLastPlayed;
}
