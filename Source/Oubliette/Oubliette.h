// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/Classes/Engine/DataTable.h"
#include "Engine/Classes/Materials/Material.h"
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
	HE_Right		UMETA(DisplayName = "Right"),
	HE_Left			UMETA(DisplayName = "Left")
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
	ESA_Wisdom		UMETA(DisplayName = "Wisdom"),
	ESA_Agility		UMETA(DisplayName = "Agility"),
	ESA_BonusFire	UMETA(DisplayName = "Bonus Fire"),
	ESA_BonusFrost	UMETA(DisplayName = "Bonus Frost"),
	ESA_BonusShock	UMETA(DisplayName = "Bonus Shock"),
	ESA_BonusShadow	UMETA(DisplayName = "Bonus Shadow")
};

UENUM(BlueprintType)
enum class EItemEffectEnum : uint8
{
	EIE_NONE
};

UENUM(BlueprintType)
enum class EItemTypeEnum : uint8
{
	ITE_Active	UMETA(DisplayName = "Active"),
	ITE_Trinket	UMETA(DisplayName = "Trinket"),
	ITE_Ring	UMETA(DisplayName = "Ring")
};

#pragma endregion ENUMS

#pragma region STRUCTS

USTRUCT(BlueprintType)
struct FActiveSpellStruct
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Active Spell")
	ESpellFormsEnum SpellType = ESpellFormsEnum::SFE_Projectile;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Active Spell")
	int32 SpellID = 0;
};

USTRUCT(BlueprintType)
struct FOffensiveSpellStruct
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Offensive Spell")
	ESpellFormsEnum SpellFormation = ESpellFormsEnum::SFE_Projectile;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Offensive Spell")
	ESpellElemsEnum SpellElement = ESpellElemsEnum::SEE_Fire;
};

USTRUCT(BlueprintType)
struct FSpellUtilStruct
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
struct FSpellProjectileStruct
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
struct FSpellChannelStruct
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
struct FSpellHitScanStruct
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
struct FStatStruct
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	int32 StatAmount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	EStatsEnum StatType;
};

USTRUCT(BlueprintType)
struct FItemStruct
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
	EItemEffectEnum Effect;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Items")
	float EffectMagnitude;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Items")
	TArray<FStatStruct> Stats;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Items")
	FText FlavourText;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Items")
	EItemTypeEnum ItemType;

	//visual stuff
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Items")
	UMaterialInterface* Material;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Items")
	FLinearColor RColour;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Items")
	FLinearColor GColour;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Items")
	FLinearColor BColour;

};

USTRUCT(BlueprintType)
struct FItemDataTableStruct : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Items")
	int32 id;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Items")
	int32 Rarity;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Items")
	FString Prefix = "rand";
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Items")
	FString Name;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Items")
	FString Suffix = "rand";
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Items")
	EItemEffectEnum Effect;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Items")
	float EffectMagnitude;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Items")
	TArray<FStatStruct> Stats;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Items")
	FText FlavourText;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Items")
	EItemTypeEnum ItemType;

	//visual stuff
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Items")
	UMaterialInterface* Material;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Items")
	FLinearColor RColour;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Items")
	FLinearColor GColour;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Items")
	FLinearColor BColour;
};

USTRUCT(BlueprintType)
struct FItemPoolStruct
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Items")
	TArray<FItemStruct> Poor;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Items")
	TArray<FItemStruct> Common;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Items")
	TArray<FItemStruct> Uncommon;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Items")
	TArray<FItemStruct> Rare;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Items")
	TArray<FItemStruct> Epic;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Items")
	TArray<FItemStruct> Legendary;
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
struct FLineageSaveStruct
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