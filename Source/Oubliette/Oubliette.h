// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/Classes/Engine/DataTable.h"
#include "Engine/Classes/Engine/Texture2D.h"
#include "Engine/Classes/Particles/ParticleSystem.h"
#include "Engine/Classes/Sound/SoundCue.h"
#include "Engine/Classes/GameFramework/DamageType.h"
#include "OublietteSpell_Projectile.h"
#include "OublietteSpell_Channel.h"
#include "Oubliette.generated.h"

#pragma region ENUMS

UENUM(BlueprintType)
enum class EHandEnum : uint8
{
	HE_Left		UMETA(DisplayName = "Left"),
	HE_Right	UMETA(DisplayName = "Right")
};

UENUM(BlueprintType)
enum class ESpellFormsEnum : uint8
{
	SFE_Projectile		UMETA(DisplayName = "Projectile"),
	SFE_Channel			UMETA(DisplayName = "Channel"),
	SFE_HitScan			UMETA(DisplayName = "Hit Scan")
};

UENUM(BlueprintType)
enum class ESpellElemsEnum : uint8
{
	SEE_Fire	UMETA(DisplayName = "Fire"),
	SEE_Frost	UMETA(DisplayName = "Frost"),
	SEE_Shock	UMETA(DisplayName = "Shock"),
	SEE_Arcane	UMETA(DisplayName = "Arcane"),
	SEE_Shadow	UMETA(DisplayName = "Shadow")
};

UENUM(BlueprintType)
enum class ESpellUtilsEnum : uint8
{
	SUE_Blink	UMETA(DisplayName = "Blink")
};

UENUM(BlueprintType)
enum class EStatsEnum : uint8
{
	ESA_NONE,
	ESA_Intellect	UMETA(DisplayName = "Intellect"),
	ESA_Wisdom		UMETA(DisplayName = "Intellect"),
	ESA_Agility		UMETA(DisplayName = "Intellect"),
	ESA_BonusFire	UMETA(DisplayName = "Intellect"),
	ESA_BonusFrost	UMETA(DisplayName = "Intellect"),
	ESA_BonusShock	UMETA(DisplayName = "Intellect"),
	ESA_BonusShadow	UMETA(DisplayName = "Intellect")
};

#pragma endregion ENUMS

#pragma region STRUCTS

USTRUCT(BlueprintType)
struct FActiveSpellStructNEW
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Active Spell")
	ESpellFormsEnum SpellType = ESpellFormsEnum::SFE_Projectile;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Active Spell")
	int32 SpellID = 0;
};

USTRUCT(BlueprintType)
struct FOffensiveSpellStructNEW
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Offensive Spell")
	ESpellFormsEnum SpellFormation = ESpellFormsEnum::SFE_Projectile;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Offensive Spell")
	ESpellElemsEnum SpellElement = ESpellElemsEnum::SEE_Fire;
};

USTRUCT(BlueprintType)
struct FSpellUtilStructNEW
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Utility Spell")
	ESpellUtilsEnum UtilitySpell;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Utility Spell")
	UParticleSystem* HandParticle;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Utility Spell")
	USoundCue* CastSound;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Utility Spell")
	FLinearColor HandColour;
};


USTRUCT(BlueprintType)
struct FSpellProjectileStructNEW
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile Spell")
	FName SpellName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile Spell")
	UParticleSystem* HandParticle;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile Spell")
	TSubclassOf<AOublietteSpell_Projectile> ProjectileClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile Spell")
	USoundCue* CastSound;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile Spell")
	FLinearColor HandColour;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile Spell")
	float BaseDamage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile Spell")
	TSubclassOf<UDamageType> DamageType;
};

USTRUCT(BlueprintType)
struct FSpellChannelStructNEW
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Channel Spell")
	FName SpellName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Channel Spell")
	UParticleSystem* HandParticle;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Channel Spell")
	UParticleSystem* DamageParticle;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Channel Spell")
	USoundCue* LoopSound;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Channel Spell")
	FLinearColor HandColour;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Channel Spell")
	float BaseDamagePerSec;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Channel Spell")
	TSubclassOf<UDamageType> DamageType;
};

USTRUCT(BlueprintType)
struct FSpellHitScanStructNEW
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hit Scan Spell")
	FName SpellName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hit Scan Spell")
	UParticleSystem* HandParticle;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hit Scan Spell")
	USoundCue* CastSound;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hit Scan Spell")
	FLinearColor HandColour;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hit Scan Spell")
	float BaseDamage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hit Scan Spell")
	TSubclassOf<UDamageType> DamageType;
};

USTRUCT(BlueprintType)
struct FStatStructNEW
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	int32 StatAmount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	EStatsEnum StatType;
};

USTRUCT(BlueprintType)
struct FItemStructNEW
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Items")
	int32 Rarity;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Items")
	FText Prefix;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Items")
	FText Name;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Items")
	FText Suffix;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Items")
	FName Effect;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Items")
	UTexture2D* IconTexture;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Items")
	FStatStructNEW Stats;
};

USTRUCT(BlueprintType)
struct FTalentToolTipStructNEW
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Talent Tooltip")
	FString Name;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Talent Tooltip")
	FText Description;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Talent Tooltip")
	FLinearColor BorderColour;
};

USTRUCT(BlueprintType)
struct FLineageSaveStructNEW
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lineage Save")
	int32 SaveID;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lineage Save")
	FString FamilyName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lineage Save")
	int32 Level;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lineage Save")
	FDateTime LastPlayed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lineage Save")
	FLinearColor SkinColor;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lineage Save")
	ESpellFormsEnum OffensiveSpellType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lineage Save")
	ESpellElemsEnum SpellElement;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lineage Save")
	ESpellUtilsEnum UtilitySpell;
};

#pragma endregion STRUCTS