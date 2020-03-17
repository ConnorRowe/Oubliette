// Fill out your copyright notice in the Description page of Project Settings.

#include "OublietteCharacter.h"
#include "Engine/World.h"

#define BASETICKRATE 0.4f

// Sets default values
AOublietteCharacter::AOublietteCharacter(const FObjectInitializer& ObjectInitializer)
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	TeamId = FGenericTeamId(0);
	
	if (GetWorld())
	{
		w = GetWorld();
		gm = (AGameModeOubliette*)w->GetAuthGameMode();
	}

	//Asset loading
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> HandMeshObj(TEXT("SkeletalMesh'/Game/Meshes/Skeletal/SK_Hand.SK_Hand'"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SpellTargetMeshObj(TEXT("StaticMesh'/Game/Meshes/Static/SM_SpellTargetArea.SM_SpellTargetArea'"));
	static ConstructorHelpers::FObjectFinder<UMaterialInterface> TargetDecalMatObj(TEXT("Material'/Game/Materials/M_SpellAreaTargetDecal.M_SpellAreaTargetDecal'"));	
	static ConstructorHelpers::FObjectFinder<USoundCue> SpellChargeCueObj(TEXT("SoundCue'/Game/Sound/Spell_Charge_Cue.Spell_Charge_Cue'")); SpellChargeCue = Cast<USoundCue>(SpellChargeCueObj.Object);

	//Component initialisation
	firstPersonCamera = ObjectInitializer.CreateDefaultSubobject<UCameraComponent>(this, TEXT("FirstPersonCamera"));
	firstPersonCamera->SetupAttachment(this->GetRootComponent());
	firstPersonCamera->SetRelativeLocation(FVector(-49.697f, 1.75f, 64.248f));
	firstPersonCamera->bUsePawnControlRotation = true;

	SK_HandL = ObjectInitializer.CreateDefaultSubobject<USkeletalMeshComponent>(this, TEXT("Hand_L"));
	SK_HandR = ObjectInitializer.CreateDefaultSubobject<USkeletalMeshComponent>(this, TEXT("Hand_R"));

	SK_HandL->SetupAttachment(firstPersonCamera);
	SK_HandL->SetRelativeLocation(FVector(61.29f, -58.0f, -51.95f));
	SK_HandL->SetRelativeRotation(FRotator(0.0f, 93.599838f, 0.0f));
	SK_HandL->SetRelativeScale3D(FVector(-0.3f, 0.3f, 0.3f));
	SK_HandL->SetSkeletalMesh(Cast<USkeletalMesh>(HandMeshObj.Object));
	SK_HandL->SetCollisionProfileName(TEXT("CharacterMesh"));
	
	SK_HandR->SetupAttachment(firstPersonCamera);
	SK_HandR->SetRelativeLocation(FVector(61.29f, 58.0f, -51.95f));
	SK_HandR->SetRelativeRotation(FRotator(0.0f, 93.599838f, 0.0f));
	SK_HandR->SetRelativeScale3D(FVector(0.3f, 0.3f, 0.3f));
	SK_HandR->SetSkeletalMesh(Cast<USkeletalMesh>(HandMeshObj.Object));
	SK_HandR->SetCollisionProfileName(TEXT("CharacterMesh"));

	spellPosL = ObjectInitializer.CreateDefaultSubobject<USceneComponent>(this, TEXT("Spell_L"));
	spellPosR = ObjectInitializer.CreateDefaultSubobject<USceneComponent>(this, TEXT("Spell_R"));

	spellPosL->SetupAttachment(SK_HandL, TEXT("Hand"));
	spellPosL->SetRelativeLocation(FVector(0.027701f, -0.676782f, -0.544008f));
	spellPosL->SetRelativeScale3D(FVector(0.005f));

	spellPosR->SetupAttachment(SK_HandR, TEXT("Hand"));
	spellPosR->SetRelativeLocation(FVector(0.027695f, -0.676783f, -0.544007));
	spellPosR->SetRelativeScale3D(FVector(0.005f));

	spellParticleL = ObjectInitializer.CreateDefaultSubobject<UParticleSystemComponent>(this, TEXT("Spell_Particles_L"));
	spellParticleR = ObjectInitializer.CreateDefaultSubobject<UParticleSystemComponent>(this, TEXT("Spell_Particles_R"));

	spellParticleL->SetupAttachment(spellPosL);
	spellParticleL->SetRelativeLocation(FVector(21.54f, 7.82f, 0.0f));
	spellParticleL->SetRelativeScale3D(FVector(-1.0f, 1.0f, 1.0f));
	spellParticleR->SetupAttachment(spellPosR);
	spellParticleR->SetRelativeLocation(FVector(21.54f, 7.82f, 0.0f));

	areaTarget = ObjectInitializer.CreateDefaultSubobject<USceneComponent>(this, TEXT("AreaTarget"));
	targetDecal = ObjectInitializer.CreateDefaultSubobject<UDecalComponent>(this, TEXT("Target Decal"));
	SM_SpellTargetArea = ObjectInitializer.CreateDefaultSubobject<UStaticMeshComponent>(this, "SM_SpellTargetArea");

	areaTarget->SetupAttachment(this->GetRootComponent());
	targetDecal->SetupAttachment(areaTarget);
	SM_SpellTargetArea->SetupAttachment(areaTarget);

	targetDecal->SetRelativeLocation(FVector(0.0f, 0.0f, -13.0f));
	targetDecal->SetRelativeRotation(FRotator(90.0f, 0.0f, 0.0f));
	targetDecal->SetMaterial(0, Cast<UMaterialInterface>(TargetDecalMatObj.Object));
	targetDecal->DecalSize = FVector(6.0f, 128.0f, 128.0f);

	SM_SpellTargetArea->SetRelativeLocation(FVector(0.0f, 0.0f, -13.0f));
	SM_SpellTargetArea->SetRelativeScale3D(FVector(0.4f));
	SM_SpellTargetArea->SetStaticMesh(Cast<UStaticMesh>(SpellTargetMeshObj.Object));
	SM_SpellTargetArea->SetCollisionProfileName(TEXT("NoCollision"));

	areaTarget->SetVisibility(false);
	targetDecal->SetVisibility(false);
	SM_SpellTargetArea->SetVisibility(false);

	ChannelSpellNiagara = ObjectInitializer.CreateDefaultSubobject<UNiagaraComponent>(this, TEXT("ChannelSpellNiagara"));
	ChannelSpellNiagara->SetupAttachment(spellPosR);
	ChannelSpellNiagara->SetVisibility(false);
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

	//Update channel spell stuff
	if (bIsAttackingR && ActiveSpellRNew.SpellFormation == ESpellFormsEnum::SFE_Channel)
	{
		float baseChannelRange = 1000.0f;
		float traceLength = baseChannelRange + (((float)ChannelRange * 0.01f) * baseChannelRange);

		FLineTraceData lineTrace = tryLineTrace(traceLength, firstPersonCamera);

		ChannelSpellNiagara->SetWorldRotation(UKismetMathLibrary::FindLookAtRotation(lineTrace.TraceStart, lineTrace.TraceEnd));

		ChannelSpellNiagara->SetVariableFloat(TEXT("User.CylinderLength"), lineTrace.Distance);
		ChannelSpellNiagara->SetVariableVec3(TEXT("User.CylinderOffset"), FVector(lineTrace.Distance, 0.0f, 0.0f));

		ChannelTarget = (lineTrace.Return_Value) ? lineTrace.Location : lineTrace.TraceEnd;
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

	//ignore all room objects as well as self
	ignoredActors.Append(gm->allRooms);

	//setup trace parameters
	const FName TraceTag("RV_Trace");
	RV_TraceParams = FCollisionQueryParams(TraceTag, false, this);
	//w->DebugDrawTraceTag = TraceTag;
	RV_TraceParams.bTraceComplex = false;
	RV_TraceParams.bReturnPhysicalMaterial = false;

	//Re-initialize hit info
	FHitResult RV_Hit(ForceInit);

	//call GetWorld() from within an actor extending class
	w->LineTraceSingleByChannel(
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

	BuffApplied(buff.Stat, buff.Name, buff.StatAmount, buff.StartTime, buff.Duration, buff.IsHidden < 1.0, buff.Icon);

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

void AOublietteCharacter::setSpellOffensive(const FOffensiveSpellStruct newSpell)
{
	ActiveSpellRNew = newSpell;

	switch (newSpell.SpellFormation)
	{
	case ESpellFormsEnum::SFE_Channel:
	{
		FSpellChannelStruct spellData = gm->Spells_Channel[(int)newSpell.SpellElement];
		spellParticleR->SetTemplate(spellData.HandParticle);
		SK_HandR->SetVectorParameterValueOnMaterials("RuneColour", FVector(spellData.HandColour));
		ChannelNiagaraAsset = Cast<UNiagaraSystem>(spellData.ChannelNiagaraSys.TryLoad());
		DamageNiagaraAsset = Cast<UNiagaraSystem>(spellData.DamageNiagaraSys.TryLoad());

		ChannelSpellNiagara->SetAsset(ChannelNiagaraAsset);

		break;
	}
	case ESpellFormsEnum::SFE_HitScan:
	{
		FSpellHitScanStruct spellData = gm->Spells_HitScan[(int)newSpell.SpellElement];
		spellParticleR->SetTemplate(spellData.HandParticle);
		SK_HandR->SetVectorParameterValueOnMaterials("RuneColour", FVector(spellData.HandColour));

		break;
	}
	case ESpellFormsEnum::SFE_Projectile:
	{
		FSpellProjectileStruct spellData = gm->Spells_Projectile[(int)newSpell.SpellElement];
		spellParticleR->SetTemplate(spellData.HandParticle);
		SK_HandR->SetVectorParameterValueOnMaterials("RuneColour", FVector(spellData.HandColour));

		break;
	}
	default:
		break;
	}
}

void AOublietteCharacter::setSpellUtility(const ESpellUtilsEnum newSpell)
{
	ActiveSpellLNew = newSpell;

	switch (newSpell)
	{
	case ESpellUtilsEnum::SUE_Blink:
	{
		FSpellUtilStruct spellData = gm->Spells_Utility[(int)newSpell];
		spellParticleL->SetTemplate(spellData.HandParticle);
		SK_HandL->SetVectorParameterValueOnMaterials("RuneColour", FVector(spellData.HandColour));

		break;
	}
	default:
		break;
	}
}

void AOublietteCharacter::chargeSpellOffensive(const FOffensiveSpellStruct spell)
{
	switch (spell.SpellFormation)
	{
	case ESpellFormsEnum::SFE_Channel:
	{
		FSpellChannelStruct spellData = gm->Spells_Channel[(int)spell.SpellElement];

		ChannelSpellNiagara->Activate();
		ChannelSpellNiagara->SetVariableVec3("User.BeamStart", spellPosR->GetComponentLocation());
		ChannelSpellNiagara->SetVariableLinearColor("User.ParticleColour", spellData.HandColour);
		ChannelSpellNiagara->SetVisibility(true);

		ChannelCurrent = spellPosR->GetComponentLocation();
		ChannelTarget = ChannelCurrent;

		channelDmgActor = w->SpawnActorDeferred<AOublietteSpell_Channel>(BP_SpellChannel, FTransform(ChannelCurrent), nullptr, this);
		UGameplayStatics::FinishSpawningActor(channelDmgActor, FTransform(ChannelCurrent));

		float tickRate = (BASETICKRATE * (ChannelTickrate * 0.01)) + BASETICKRATE;

		channelDmgActor->initChannel(spellData.BaseDamagePerSec, spellData.DamageType, tickRate, DoubleRadius * 0.01, spellData.HandColour, DamageNiagaraAsset);

		channelSound = UGameplayStatics::SpawnSoundAttached(spellData.LoopSound, spellPosR);

		break;
	}
	case ESpellFormsEnum::SFE_HitScan:
	{
		UGameplayStatics::PlaySoundAtLocation(this, SpellChargeCue, spellPosR->GetComponentLocation());

		break;
	}
	case ESpellFormsEnum::SFE_Projectile:
	{
		
		UGameplayStatics::PlaySoundAtLocation(this, SpellChargeCue, spellPosR->GetComponentLocation());

		break;
	}
	default:
		break;
	}
}

void AOublietteCharacter::finishSpellOffensive(const FOffensiveSpellStruct spell)
{
	switch (spell.SpellFormation)
	{
	case ESpellFormsEnum::SFE_Channel:
	{
		ChannelSpellNiagara->SetVisibility(false);
		channelSound->Stop();
		channelDmgActor->finish();
	}
	default:
		break;
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
