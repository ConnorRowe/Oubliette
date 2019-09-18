// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/Classes/Engine/DataTable.h"
#include "Engine/Classes/Materials/Material.h"
#include "Engine/Classes/Particles/ParticleSystem.h"
#include "Engine/Classes/Sound/SoundCue.h"
#include "Engine/Classes/GameFramework/DamageType.h"
#include "Engine/Texture.h"
#include "OublietteSpell_Projectile.h"
#include "OublietteSpell_Channel.h"
#include "Oubliette.generated.h"

#define GETENUMSTRING(etype, evalue) ( (FindObject<UEnum>(ANY_PACKAGE, TEXT(etype), true) != nullptr) ? FindObject<UEnum>(ANY_PACKAGE, TEXT(etype), true)->GetEnumName((int32)evalue) : FString("Invalid - are you sure enum uses UENUM() macro?") )

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
	ESA_Intellect				UMETA(DisplayName = "Intellect"),
	ESA_Wisdom					UMETA(DisplayName = "Wisdom"),
	ESA_Agility					UMETA(DisplayName = "Agility"),
	ESA_BonusFire				UMETA(DisplayName = "Bonus Fire Damage"),
	ESA_BonusFrost				UMETA(DisplayName = "Bonus Frost Damage"),
	ESA_BonusShock				UMETA(DisplayName = "Bonus Shock Damage"),
	ESA_BonusArcane 			UMETA(DisplayName = "Bonus Arcane Damage"),
	ESA_BonusShadow				UMETA(DisplayName = "Bonus Shadow Damage"),
	ESA_BonusXP					UMETA(DisplayName = "% Bonus XP"),
	ESA_BetterLoot				UMETA(DisplayName = "% Better Chance of Finding Magical Items"),
	ESA_DmgUndead				UMETA(DisplayName = "% Bonus Damage against Undead"),
	ESA_DmgSlime				UMETA(DisplayName = "% Bonus Damage against Slimes"),
	ESA_DmgBeasts				UMETA(DisplayName = "% Bonus Damage against Beasts"),
	ESA_SpawnEye				UMETA(DisplayName = "% chance to Spawn Eye"),
	ESA_MagicSnails 			UMETA(DisplayName = "guarenteed Magic Snails"),
	ESA_ShroomBonus 			UMETA(DisplayName = "% Shroom Bonus"),
	ESA_NiceRats				UMETA(DisplayName = "Nice Rats"),
	ESA_IgnoreDmg				UMETA(DisplayName = "% to Ignore Damage"),
	ESA_ProjSpeed				UMETA(DisplayName = "% faster Projectile Speed"),
	ESA_SpellPierce				UMETA(DisplayName = "% chance for Spell Pierce"),
	ESA_DoubleRadius			UMETA(DisplayName = "Double Radius Chance"),
	ESA_BonusBurn				UMETA(DisplayName = "% bonus chance to ignite enemies"),
	ESA_BonusChill				UMETA(DisplayName = "% bonus chance to chill enemies"),
	ESA_BonusStun				UMETA(DisplayName = "% bonus chance to stun enemies"),
	ESA_RegenManaChunkOnKill	UMETA(DisplayName = "% chance to regenerate 10% mana on kill"),
	ESA_CastSpeed				UMETA(DisplayName = "% faster cast speed"),
	ESA_ChannelTickrate			UMETA(DisplayName = "% faster damage tick rate"),
	ESA_ChannelRange			UMETA(DisplayName = "% faster further range"),
	ESA_ArcToEnemy				UMETA(DisplayName = "% chance to arc to a nearby enemy"),
	ESA_DamageShield			UMETA(DisplayName = "While stat is at 1.0, can absorb one attack"),
	ESA_ManaCostReduction		UMETA(DisplayName = "Reduces mana costs")

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

UENUM(BlueprintType)
enum class EEnemyTypesEnum : uint8
{
	ETE_Slime		UMETA(DisplayName = "Slime"),
	ETE_Slime_Giant	UMETA(DisplayName = "Slime Giant")
};

UENUM(BlueprintType)
enum class ERaceEnum : uint8
{
	ERE_Human	UMETA(DisplayName = "Human")
};

UENUM(BlueprintType)
enum class EStatusEffectEnum : uint8
{
	ESEE_Bleeding	UMETA(DisplayName = "Bleeding"),
	ESEE_Burning	UMETA(DisplayName = "Burning"),
	ESEE_Chilled	UMETA(DisplayName = "Chilled"),
	ESEE_Stunned	UMETA(DisplayName = "Stunned")
};

UENUM(BlueprintType)
enum class EBuffEnum : uint8
{
	EBE_ManaShield	UMETA(DisplayName = "Mana Shield"),
	EBE_Clearcasting UMETA(DisplayName = "Clearcasting")
};

UENUM(BlueprintType)
enum class EBuffSourceEnum : uint8
{
	EBSE_OnKill			UMETA(DisplayName = "On Kill"),
	EBSE_OnHit			UMETA(DisplayName = "On Hit"),
	EBSE_OnTakeDamage	UMETA(DisplayName = "On Take Damage"),
	EBSE_Reccuring		UMETA(DisplayName = "Reccuring"),
	EBSE_OnCast			UMETA(DisplayName = "On Cast")
};

UENUM(BlueprintType)
enum class ESortMethodEnum : uint8
{
	ESME_Name			UMETA(DisplayName = "Name"),
	ESME_Level			UMETA(DisplayName = "Level"),
	ESME_DateTime		UMETA(DisplayName = "DateTime")
};


#pragma endregion ENUMS

#pragma region STRUCTS

USTRUCT(BlueprintType)
struct FIconStruct
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Materials")
	UTexture* Icon;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Materials")
	FLinearColor RColour;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Materials")
	FLinearColor GColour;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Materials")
	FLinearColor BColour;
};

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
	FSoftObjectPath ChannelNiagaraSys;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Channel Spell")
	FSoftObjectPath DamageNiagaraSys;
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
	UTexture* Icon;
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
	UTexture* Icon;
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
struct FItemAffixStruct : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Items")
	FName Affix;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Items")
	TArray<FStatStruct> Stats;
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
struct FAncestorStruct
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lineage Save")
	FName FirstName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lineage Save")
	int32 Level;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lineage Save")
	FDateTime DateTimeKilled;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lineage Save")
	FName KilledByName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lineage Save")
	FName LevelName;
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
	int32 Experience;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lineage Save")
	int32 SkillPoints;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lineage Save")
	ERaceEnum Race;
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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lineage Save")
	TArray<FIntPoint> FormationTree;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lineage Save")
	TArray<FIntPoint> ElementTree;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lineage Save")
	TArray<FIntPoint> GeneralTree;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lineage Save")
	TArray<FAncestorStruct> Ancestors;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lineage Save")
	FName FirstName;
};

USTRUCT(BlueprintType)
struct FBuffStruct
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Talent Buff")
	EBuffEnum Buff;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Talent Buff")
	FName Name;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Talent Buff")
	EBuffSourceEnum Source;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Talent Buff")
	EStatsEnum Stat;

	// 0.0 on a val means it takes it from the skill's modified point value
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Talent Buff")
	float Chance;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Talent Buff")
	float Power;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Talent Buff")
	float DurationSeconds;
	
	//Rendering stuff
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Talent Buff")
	bool isHidden;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Talent Buff")
	FIconStruct BuffIcon;
};


#pragma endregion STRUCTS

// Load Blueprint Class Asset From Path
static FORCEINLINE UClass* LoadBPFromPath(FString Path)
{
	UObject* loadObj = StaticLoadObject(UObject::StaticClass(), nullptr, *Path);

	UBlueprint* bpObj = Cast<UBlueprint>(loadObj);

	return CastChecked<UClass>(bpObj->GeneratedClass);
}
