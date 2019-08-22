// Fill out your copyright notice in the Description page of Project Settings.


#include "GameInstanceOubliette.h"

void UGameInstanceOubliette::ApplySkillStats()
{
	for (auto & Stat : SkillStats)
	{
		switch (Stat.StatType)
		{
		case EStatsEnum::ESA_ArcToEnemy:
			charRef->ArcToEnemy += Stat.StatAmount;
			break;
		case EStatsEnum::ESA_BetterLoot:
			charRef->BonLoot += Stat.StatAmount;
			break;
		case EStatsEnum::ESA_BonusBurn:
			charRef->BonusBurn += Stat.StatAmount;
			break;
		case EStatsEnum::ESA_BonusChill:
			charRef->BonusChill += Stat.StatAmount;
			break;
		case EStatsEnum::ESA_BonusStun:
			charRef->BonusStun += Stat.StatAmount;
			break;
		case EStatsEnum::ESA_BonusXP:
			charRef->BonXP += Stat.StatAmount;
			break;
		case EStatsEnum::ESA_CastSpeed:
			charRef->CastSpeed += Stat.StatAmount;
			break;
		case EStatsEnum::ESA_ChannelRange:
			charRef->ChannelRange += Stat.StatAmount;
			break;
		case EStatsEnum::ESA_ChannelTickrate:
			charRef->ChannelTickrate += Stat.StatAmount;
			break;
		case EStatsEnum::ESA_DoubleRadius:
			charRef->DoubleRadius += Stat.StatAmount;
			break;
		case EStatsEnum::ESA_IgnoreDmg:
			charRef->IgnoreDmg += Stat.StatAmount;
			break;
		case EStatsEnum::ESA_MagicSnails:
			charRef->MagicSnails += Stat.StatAmount;
			break;
		case EStatsEnum::ESA_NiceRats:
			charRef->NiceRats += Stat.StatAmount;
			break;
		case EStatsEnum::ESA_ProjSpeed:
			charRef->ProjSpeed += Stat.StatAmount;
			break;
		case EStatsEnum::ESA_RegenManaChunkOnKill:
			charRef->RegenManaChunkOnKill += Stat.StatAmount;
			break;
		case EStatsEnum::ESA_ShroomBonus:
			charRef->ShroomBonus += Stat.StatAmount;
			break;
		case EStatsEnum::ESA_SpawnEye:
			charRef->SpawnEyeOnKill += Stat.StatAmount;
			break;
		case EStatsEnum::ESA_SpellPierce:
			charRef->SpellPierce += Stat.StatAmount;
			break;

		default:
			break;
		}
	}

}

void UGameInstanceOubliette::ClearTrackedVars()
{
	Track_EnemiesKilled = 0;
	Track_FloorsComplete = 0;
	Track_ItemsFound = 0;
}
