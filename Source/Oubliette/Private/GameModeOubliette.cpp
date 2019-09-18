// Fill out your copyright notice in the Description page of Project Settings.

#include "OublietteCharacter.h"
#include "GameModeOubliette.h"

FItemStruct AGameModeOubliette::GenerateItemFromPool(const FItemPoolStruct ItemPool)
{
	// get random float and add the character's bonus loot stat to it
	float rarity = (FMath::FRand());
	rarity *= 1.0f + ((float)charRef->BonLoot / 100.0f);

	TArray<FItemStruct> possibleItems;

	if (rarity <= 0.45f)
	{
		possibleItems = ItemPool.Common;
	}
	else if (rarity > 0.45f && rarity <= 0.7f)
	{
		possibleItems = ItemPool.Uncommon;
	}
	else if (rarity > 0.7f && rarity <= 0.85f)
	{
		possibleItems = ItemPool.Rare;
	}
	else if (rarity > 0.85f && rarity <= 0.95f)
	{
		possibleItems = ItemPool.Epic;
	}
	else if (rarity > 0.95f)
	{
		possibleItems = ItemPool.Legendary;
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
