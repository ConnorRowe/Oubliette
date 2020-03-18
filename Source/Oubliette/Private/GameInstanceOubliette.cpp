// Fill out your copyright notice in the Description page of Project Settings.


#include "GameInstanceOubliette.h"

void UGameInstanceOubliette::ApplySkillStatsAndBuffs()
{
	for (auto& Stat : SkillStats)
	{
		charRef->addToStat(Stat.StatType, Stat.StatAmount);
	}

	for (auto& Buff : SkillBuffs)
	{
		switch (Buff.Source)
		{
		case EBuffSourceEnum::EBSE_Reccuring:
		{
			//Reccuring buffs use a different method of application so they directly are constructed with the FCurrentBuff struct
			FCurrentBuff cBuff;
			cBuff.Duration = Buff.DurationSeconds;
			cBuff.StartTime = 0.0f;
			cBuff.Name = Buff.Name;
			cBuff.Stat = Buff.Stat;
			cBuff.StatAmount = Buff.Power;
			cBuff.Icon = Buff.BuffIcon;
			cBuff.IsHidden = Buff.isHidden;
			charRef->Buffs_Reccuring.Add(cBuff);
			break;
		}
		case EBuffSourceEnum::EBSE_OnCast:
			charRef->Buffs_OnCast.Add(Buff);
			break;
		case EBuffSourceEnum::EBSE_OnHit:
			charRef->Buffs_OnHit.Add(Buff);
			break;
		case EBuffSourceEnum::EBSE_OnKill:
			charRef->Buffs_OnKill.Add(Buff);
			break;
		case EBuffSourceEnum::EBSE_OnTakeDamage:
			charRef->Buffs_OnTakeDamage.Add(Buff);
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
