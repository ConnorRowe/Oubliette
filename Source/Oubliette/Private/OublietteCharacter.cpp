// Fill out your copyright notice in the Description page of Project Settings.

#include "OublietteCharacter.h"
#include "Engine/World.h"

// Sets default values
AOublietteCharacter::AOublietteCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	TeamId = FGenericTeamId(0);
	
	if (GetWorld())
	{
		gm = (AGameModeOubliette*)GetWorld()->GetAuthGameMode();
	}
}

// Called when the game starts or when spawned
void AOublietteCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AOublietteCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	updateCurrentBuffs();

	//Regenerate Mana
	if (ManaCurrent < ManaMax)
	{
		ManaCurrent += (baseManaRegen*(1.0f + (float)ManaRegenPercent) + (float)ManaRegen)*DeltaTime;
	}

	//Clamp Mana
	if (ManaCurrent > ManaMax)
	{
		ManaCurrent = ManaMax;
	}
}

// Called to bind functionality to input
void AOublietteCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

FGenericTeamId AOublietteCharacter::GetGenericTeamId() const
{
	return TeamId;
}

void AOublietteCharacter::BuffApplied_Implementation(EStatsEnum BuffStat, FName BuffName, float StatAmount, float StartTime, float Duration, bool IsHidden, FIconStruct Icon)
{
}

void AOublietteCharacter::BuffRemoved_Implementation(EStatsEnum BuffStat, FName BuffName, float StatAmount, float StartTime, float Duration)
{
}

//attempt a single line trace from a given component
FLineTraceData AOublietteCharacter::tryLineTrace(float traceLength, USceneComponent* startComp)
{
	TArray<AActor*> ignoredActors;
	FCollisionQueryParams RV_TraceParams;
	FVector tStart = startComp->GetComponentLocation();
	FVector tEnd = tStart + (startComp->GetForwardVector() * traceLength);

	//get game mode
	AGameModeOubliette* gm = (AGameModeOubliette*)GetWorld()->GetAuthGameMode();

	//ignore all room objects as well as self
	ignoredActors.Append(gm->allRooms);

	//setup trace parameters
	RV_TraceParams = FCollisionQueryParams(FName(TEXT("RV_Trace")), false, this);
	RV_TraceParams.bTraceComplex = false;
	RV_TraceParams.bReturnPhysicalMaterial = false;

	//Re-initialize hit info
	FHitResult RV_Hit(ForceInit);

	//call GetWorld() from within an actor extending class
	GetWorld()->LineTraceSingleByChannel(
		RV_Hit,			//result
		tStart,			//start
		tEnd,			//end
		ECC_Visibility,	//collision channel
		RV_TraceParams
	);

	FLineTraceData tData;

	tData.Location = RV_Hit.Location;
	tData.Return_Value = RV_Hit.bBlockingHit;
	tData.TraceEnd = RV_Hit.TraceEnd;
	tData.TraceStart = RV_Hit.TraceStart;
	tData.Distance = RV_Hit.Distance;
	tData.HitActor = RV_Hit.Actor;
	
	return tData;
}

void AOublietteCharacter::calculateStats()
{
	//Reset stats
	Inte = baseInte;
	Agil = baseAgil;
	Wisd = baseWisd;
	ManaMax = baseMana;

	//Loop through every stat and set to 0
	for (uint8 i = 0; i < static_cast<uint8>(EStatsEnum::ESA_Last); ++i)
	{
		int32* stat = GetStat(static_cast<EStatsEnum>(i));
		if (stat != nullptr)
		{
			*stat = 0;
		}
	}

	//Parse items to find stats
	for (auto& Item : Inventory)
	{
		for (auto& Stat : Item.Stats)
		{
			addToStat(Stat.StatType, Stat.StatAmount);
		}
	}

	//Calculate Mana --- 1 extra mana per intellect
	ManaMax += Inte;

}

void AOublietteCharacter::calcBaseDamage()
{

	float elementalBonus;
	float baseDamage;
	int spellID;

	// Find the correct elemental damage bonus multiplier
	switch (ActiveSpellRNew.SpellElement)
	{
	case ESpellElemsEnum::SEE_Arcane:
		elementalBonus = BonArcane;
		spellID = 3;
		break;

	case ESpellElemsEnum::SEE_Fire:
		elementalBonus = BonFire;
		spellID = 0;
		break;

	case ESpellElemsEnum::SEE_Frost:
		elementalBonus = BonFrost;
		spellID = 1;
		break;

	case ESpellElemsEnum::SEE_Shadow:
		elementalBonus = BonShadow;
		spellID = 4;
		break;

	case ESpellElemsEnum::SEE_Shock:
		elementalBonus = BonShock;
		spellID = 2;
		break;
	
	default:
		break;
	}

	// Find the correct base damage
	switch (ActiveSpellRNew.SpellFormation)
	{
	case ESpellFormsEnum::SFE_Channel:
		baseDamage = gm->Spells_Channel[spellID].BaseDamagePerSec;
		break;

	case ESpellFormsEnum::SFE_HitScan:
		baseDamage = gm->Spells_HitScan[spellID].BaseDamage;
		break;

	case ESpellFormsEnum::SFE_Projectile:
		baseDamage = gm->Spells_Projectile[spellID].BaseDamage;
		break;

	default:
		break;
	}

	BaseSpellDamage = baseDamage * (1.0f + (float)Inte / 10.0f) * (1.0f + (elementalBonus/100.0f));
}

FSpellDamageCalc AOublietteCharacter::calcSpellDamage()
{
	FSpellDamageCalc spellDmg;
	
	float critChance = 0.02f + (Wisd / 150.0f);

	spellDmg.Damage = FMath::RandRange(BaseSpellDamage*0.8f, BaseSpellDamage*1.2f);


	if (FMath::FRand() < critChance)
	{
		spellDmg.Damage = spellDmg.Damage * 2;
		spellDmg.isCrit = true;
	}

	return spellDmg;
}

bool AOublietteCharacter::TrySpendMana(EHandEnum Hand)
{
	float manaCost = 0.0f;
	bool isSuccess = false;

	//Select a mana cost depending on the spell type
	if (Hand == EHandEnum::HE_Left)
	{
		manaCost = 25.0f;
	}
	else if (Hand == EHandEnum::HE_Right)
	{
		switch (ActiveSpellRNew.SpellFormation)
		{
		case ESpellFormsEnum::SFE_Channel:
		{
			manaCost = 5.0f;
			break;
		}
		case ESpellFormsEnum::SFE_HitScan:
		{
			manaCost = 10.0f;
			break;
		}
		case ESpellFormsEnum::SFE_Projectile:
		{
			manaCost = 10.0f;
			break;
		}
		}
	}

	//Apply mana cost reduction stat
	manaCost *= (1.0f - ManaCostReduction);

	//Reduce mana
	ManaCurrent -= manaCost;

	//Check if all mana has been spent, spell is only successful if mana has not gone negative
	if (ManaCurrent < 0.0f)
	{
		ManaCurrent = 0.0f;
	}
	else
	{
		isSuccess = true;
	}

	return isSuccess;
}

void AOublietteCharacter::addToStat(EStatsEnum Stat, float Amount)
{
	int32* stat = GetStat(Stat);
	int32 roundedAmount = FMath::RoundToInt(Amount);
	//Ensures the stat is valid
	if (stat != nullptr)
	{
		*stat = *stat + roundedAmount;
	}
}

void AOublietteCharacter::applyBuff(const FBuffStruct & buff)
{
	FCurrentBuff newBuff;
	newBuff.Name = buff.Name;
	newBuff.StartTime = GetWorld()->GetRealTimeSeconds();
	newBuff.Duration = buff.DurationSeconds;
	newBuff.Stat = buff.Stat;
	newBuff.StatAmount = buff.Power;
	newBuff.IsHidden = buff.isHidden;
	newBuff.Icon = buff.BuffIcon;

	applyBuff(newBuff);
}

void AOublietteCharacter::applyBuff(const FCurrentBuff & buff)
{
	addToStat(buff.Stat, buff.StatAmount);

	CurrentBuffs.Add(buff);

	BuffApplied(buff.Stat, buff.Name, buff.StatAmount, buff.StartTime, buff.Duration, buff.IsHidden, buff.Icon);

	FString bufflog;
	bufflog = "Buff Applied: " + GETENUMSTRING("EStatsEnum", buff.Stat) + ", " + FString::SanitizeFloat(buff.StatAmount) + ", for " + FString::SanitizeFloat(buff.Duration) + " seconds";
	UE_LOG(LogTemp, Warning, TEXT("___ %s ___ "), *bufflog);
}

void AOublietteCharacter::removeBuff(const FCurrentBuff & buff)
{
	addToStat(buff.Stat, buff.StatAmount*-1.0f);

	FString bufflog;
	bufflog = "Buff Removed: " + GETENUMSTRING("EStatsEnum", buff.Stat) + ", " + FString::SanitizeFloat(buff.StatAmount) + ", for " + FString::SanitizeFloat(buff.Duration) + " seconds";
	UE_LOG(LogTemp, Warning, TEXT("___ %s ___ "), *bufflog);

	BuffRemoved(buff.Stat, buff.Name, buff.StatAmount, buff.StartTime, buff.Duration);

	CurrentBuffs.RemoveSingle(buff);
}

void AOublietteCharacter::updateCurrentBuffs()
{
	float time = GetWorld()->GetTimeSeconds();

	//Update current buffs
	if (CurrentBuffs.Num() > 0)
	{
		for (const auto& Buff : CurrentBuffs)
		{
			if (time >= (Buff.StartTime + Buff.Duration))
			{
				removeBuff(Buff);
				break;
			}
		}
	}

	//Update reccuring buffs, which are not removed and constantly reapply themselves
	if (Buffs_Reccuring.Num() > 0)
	{
		for (FCurrentBuff & Buff : Buffs_Reccuring)
		{
			if ((time >= (Buff.StartTime + Buff.Duration)) && !buffExistsByName(Buff.Name))
			{
				Buff.StartTime = time;
				applyBuff(Buff);
			}
		}
	}
}

void AOublietteCharacter::tryActivateBuff(const EBuffSourceEnum & Source)
{
	TArray<FBuffStruct>* Buffs = nullptr;

	switch (Source)
	{
	case EBuffSourceEnum::EBSE_OnCast:
		Buffs = &Buffs_OnCast;
		break;
	case EBuffSourceEnum::EBSE_OnHit:
		Buffs = &Buffs_OnHit;
		break;
	case EBuffSourceEnum::EBSE_OnKill:
		Buffs = &Buffs_OnKill;
		break;
	case EBuffSourceEnum::EBSE_OnTakeDamage:
		Buffs = &Buffs_OnTakeDamage;
		break;

	default:
		break;
	}

	//Ensure pointer is valid
	check(Buffs);

	for (auto Buff : *Buffs)
	{
		float r = FMath::RandRange(0.0f,1.0f);

		if (Buff.Chance > 0.0f && r <= Buff.Chance)
		{
			applyBuff(Buff);
		}
	}
}

//Searches for a buff with the given FName and returns true if it was found
bool AOublietteCharacter::buffExistsByName(const FName Name)
{
	bool hasFoundBuff = false;

	for (const auto & Buff : CurrentBuffs)
	{
		if (Buff.Name == Name)
		{
			hasFoundBuff = true;
			break;
		}
	}

	return hasFoundBuff;
}

//Attempts to remove a buff with the given FName
void AOublietteCharacter::RemoveBuffByName(const FName Name)
{
	FCurrentBuff* foundBuff = nullptr;

	if (buffExistsByName(Name))
	{
		//Get a pointer to the buff
		for (FCurrentBuff Buff : CurrentBuffs)
		{
			if (Buff.Name == Name)
			{
				foundBuff = &Buff;
				break;
			}
		}

		//Remove the buff
		removeBuff(*foundBuff);
	}
}

//Returns the stat variable for the given Stat enum
int32* AOublietteCharacter::GetStat(EStatsEnum Stat)
{
	switch (Stat)
	{
	case EStatsEnum::ESA_Agility:
		return &Agil;

	case EStatsEnum::ESA_BetterLoot:
		return &BonLoot;

	case EStatsEnum::ESA_BonusArcane:
		return &BonArcane;

	case EStatsEnum::ESA_BonusFire:
		return &BonFire;

	case EStatsEnum::ESA_BonusFrost:
		return &BonFrost;
		
	case EStatsEnum::ESA_BonusShadow:
		return &BonShadow;

	case EStatsEnum::ESA_BonusShock:
		return &BonShock;

	case EStatsEnum::ESA_BonusXP:
		return &BonXP;

	case EStatsEnum::ESA_DmgUndead:
		return &BonUndead;

	case EStatsEnum::ESA_DmgBeasts:
		return &BonBeast;

	case EStatsEnum::ESA_DmgSlime:
		return &BonSlime;

	case EStatsEnum::ESA_Intellect:
		return &Inte;

	case EStatsEnum::ESA_Wisdom:
		return &Wisd;

	case EStatsEnum::ESA_SpawnEye:
		return &SpawnEyeOnKill;

	case EStatsEnum::ESA_MagicSnails:
		return &MagicSnails;

	case EStatsEnum::ESA_ShroomBonus:
		return &NiceRats;

	case EStatsEnum::ESA_NiceRats:
		return &ShroomBonus;

	case EStatsEnum::ESA_IgnoreDmg:
		return &IgnoreDmg;

	case EStatsEnum::ESA_ProjSpeed:
		return &ProjSpeed;

	case EStatsEnum::ESA_SpellPierce:
		return &SpellPierce;

	case EStatsEnum::ESA_DoubleRadius:
		return &DoubleRadius;

	case EStatsEnum::ESA_BonusBurn:
		return &BonusBurn;
		
	case EStatsEnum::ESA_BonusChill:
		return &BonusChill;

	case EStatsEnum::ESA_BonusStun:
		return &BonusStun;

	case EStatsEnum::ESA_RegenManaChunkOnKill:
		return &RegenManaChunkOnKill;

	case EStatsEnum::ESA_CastSpeed:
		return &CastSpeed;

	case EStatsEnum::ESA_ChannelTickrate:
		return &ChannelTickrate;

	case EStatsEnum::ESA_ChannelRange:
		return &ChannelRange;

	case EStatsEnum::ESA_ArcToEnemy:
		return &ArcToEnemy;

	case EStatsEnum::ESA_DamageShield:
		return &DamageShield;

	case EStatsEnum::ESA_ManaCostReduction:
		return &ManaCostReduction;

	case EStatsEnum::ESA_BonusMana:
		return &BonusMana;

	case EStatsEnum::ESA_BonusManaPercent:
		return &BonusManaPercent;

	case EStatsEnum::ESA_ManaRegen:
		return &ManaRegen;

	case EStatsEnum::ESA_ManaRegenPercent:
		return &ManaRegenPercent;

	default:
		return nullptr;
	}
}
