// Fill out your copyright notice in the Description page of Project Settings.


#include "GameModeOubliette.h"

FItemStruct AGameModeOubliette::GenerateItemFromPool(const FItemPoolStruct ItemPool)
{
	const int rarity = FMath::RandRange(0, 4);

	TArray<FItemStruct> possibleItems;

	switch (rarity)
	{
	case 0:
		possibleItems = ItemPool.Common;
		break;
	case 1:
		possibleItems = ItemPool.Uncommon;
		break;
	case 2:
		possibleItems = ItemPool.Rare;
		break;
	case 3:
		possibleItems = ItemPool.Epic;
		break;
	case 4:
		possibleItems = ItemPool.Legendary;
		break;
	}
	
	FItemStruct newItem = possibleItems[FMath::RandRange(0,possibleItems.Num()-1)];
	const FText & randTxt = FText::FromString("rand");

	if (newItem.Prefix.EqualTo(randTxt))
	{
		// only add a prefix sometimes
		if (FMath::RandBool())
		{
			const FItemAffixStruct prefix = ItemPrefixes[FMath::RandRange(0, ItemPrefixes.Num() - 1)];

			newItem.Prefix = FText::FromName(prefix.Affix);
			newItem.Stats.Append(prefix.Stats);
		}
		else
		{
			newItem.Prefix = FText();
		}
	}

	if (newItem.Suffix.EqualTo(randTxt))
	{
		// only add a suffix sometimes
		if (FMath::RandBool())
		{
			const FItemAffixStruct suffix = ItemSuffixes[FMath::RandRange(0, ItemSuffixes.Num() - 1)];

			newItem.Suffix = FText::FromName(suffix.Affix);
			newItem.Stats.Append(suffix.Stats);
		}
		else
		{
			newItem.Suffix = FText();
		}
	}

	return newItem;
}
