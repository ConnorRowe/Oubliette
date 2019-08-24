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

void AOublietteCharacter::BuffApplied_Implementation(EStatsEnum BuffStat, FName BuffName, float StatAmount, float StartTime, float Duration)
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
	BonFire = baseBonFire;
	BonFrost = baseBonFire;
	BonShock = baseBonShock;
	BonArcane = baseBonArcane;
	BonShadow = baseBonShadow;
	BonUndead = baseBonUndead;
	BonSlime = baseBonSlime;
	BonBeast = baseBonBeast;
	BonXP = baseBonXP;
	BonLoot = baseBonLoot;

	//Parse items to find stats
	for (auto& Item : Inventory)
	{
		for (auto& Stat : Item.Stats)
		{
			addToStat(Stat.StatType, Stat.StatAmount);
		}
	}
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

void AOublietteCharacter::addToStat(EStatsEnum Stat, float Amount)
{
	switch (Stat)
	{
	case EStatsEnum::ESA_Agility:
		Agil += Amount;
		break;
	case EStatsEnum::ESA_BetterLoot:
		BonLoot += Amount;
		break;
	case EStatsEnum::ESA_BonusArcane:
		BonArcane += Amount;
		break;
	case EStatsEnum::ESA_BonusFire:
		BonFire += Amount;
		break;
	case EStatsEnum::ESA_BonusFrost:
		BonFrost += Amount;
		break;
	case EStatsEnum::ESA_BonusShadow:
		BonShadow += Amount;
		break;
	case EStatsEnum::ESA_BonusShock:
		BonShock += Amount;
		break;
	case EStatsEnum::ESA_BonusXP:
		BonXP += Amount;
		break;
	case EStatsEnum::ESA_DmgUndead:
		BonUndead += Amount;
		break;
	case EStatsEnum::ESA_DmgBeasts:
		BonBeast += Amount;
		break;
	case EStatsEnum::ESA_DmgSlime:
		BonSlime += Amount;
		break;
	case EStatsEnum::ESA_Intellect:
		Inte += Amount;
		break;
	case EStatsEnum::ESA_Wisdom:
		Wisd += Amount;
		break;
	case EStatsEnum::ESA_SpawnEye:
		SpawnEyeOnKill += Amount;
		break;
	case EStatsEnum::ESA_MagicSnails:
		MagicSnails += Amount;
		break;
	case EStatsEnum::ESA_ShroomBonus:
		NiceRats += Amount;
		break;
	case EStatsEnum::ESA_NiceRats:
		ShroomBonus += Amount;
		break;
	case EStatsEnum::ESA_IgnoreDmg:
		IgnoreDmg += Amount;
		break;
	case EStatsEnum::ESA_ProjSpeed:
		ProjSpeed += Amount;
		break;
	case EStatsEnum::ESA_SpellPierce:
		SpellPierce += Amount;
		break;
	case EStatsEnum::ESA_DoubleRadius:
		DoubleRadius += Amount;
		break;
	case EStatsEnum::ESA_BonusBurn:
		BonusBurn += Amount;
		break;
	case EStatsEnum::ESA_BonusChill:
		BonusChill += Amount;
		break;
	case EStatsEnum::ESA_BonusStun:
		BonusStun += Amount;
		break;
	case EStatsEnum::ESA_RegenManaChunkOnKill:
		RegenManaChunkOnKill += Amount;
		break;
	case EStatsEnum::ESA_CastSpeed:
		CastSpeed += Amount;
		break;
	case EStatsEnum::ESA_ChannelTickrate:
		ChannelTickrate += Amount;
		break;
	case EStatsEnum::ESA_ChannelRange:
		ChannelRange += Amount;
		break;
	case EStatsEnum::ESA_ArcToEnemy:
		ArcToEnemy += Amount;
		break;
	case EStatsEnum::ESA_DamageShield:
		DamageShield += Amount;
		break;
	case EStatsEnum::ESA_ManaCostReduction:
		ManaCostReduction += Amount;
		break;
	default:
		break;
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

	addToStat(buff.Stat, buff.Power);

	CurrentBuffs.Add(newBuff);

	BuffApplied(buff.Stat, buff.Name, buff.Power, newBuff.StartTime, buff.DurationSeconds);

	FString bufflog;
	bufflog = "Buff Applied: " + GETENUMSTRING("EStatsEnum", buff.Stat) + ", " + FString::SanitizeFloat(buff.Power) + ", for " + FString::SanitizeFloat(buff.DurationSeconds) + " seconds";
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
	if (CurrentBuffs.Num() > 0)
	{
		for (const auto& Buff : CurrentBuffs)
		{
			if (GetWorld()->GetRealTimeSeconds() >= (Buff.StartTime + Buff.Duration))
			{
				removeBuff(Buff);
				break;
			}
		}
	}
}

void AOublietteCharacter::tryActivateBuff(const EBuffSourceEnum & Source)
{
	TArray<FBuffStruct>* Buffs = nullptr;

	switch (Source)
	{
	case EBuffSourceEnum::EBSE_EveryXMinutes:
		Buffs = &Buffs_EveryXMinutes;
		break;
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
