// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Oubliette.h"
#include "MyDamageType_Arcane.h"
#include "MyDamageType_Fire.h"
#include "MyDamageType_Frost.h"
#include "MyDamageType_Shadow.h"
#include "MyDamageType_Shock.h"
#include "GameModeOubliette.h"
#include "CollisionQueryParams.h"
#include "GameFramework/Character.h"
#include "GameFramework/DamageType.h"
#include "GenericTeamAgentInterface.h"
#include "OublietteCharacter.generated.h"

USTRUCT(BlueprintType)
struct FLineTraceData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(BlueprintReadOnly, Category = "Line Trace")
	bool Return_Value;
	UPROPERTY(BlueprintReadOnly, Category = "Line Trace")
	FVector Location;
	UPROPERTY(BlueprintReadOnly, Category = "Line Trace")
	FVector TraceStart;
	UPROPERTY(BlueprintReadOnly, Category = "Line Trace")
	FVector TraceEnd;
	UPROPERTY(BlueprintReadOnly, Category = "Line Trace")
	TWeakObjectPtr<class AActor> HitActor;
	UPROPERTY(BlueprintReadOnly, Category = "Line Trace")
	float Distance;
};

USTRUCT(BlueprintType)
struct FSpellDamageCalc
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(BlueprintReadOnly, Category = "Character | Spells")
	float Damage;
	UPROPERTY(BlueprintReadOnly, Category = "Character | Spells")
	bool isCrit = false;
};


UCLASS()
class OUBLIETTE_API AOublietteCharacter : public ACharacter, public IGenericTeamAgentInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AOublietteCharacter();

private:
	FGenericTeamId TeamId;
	virtual FGenericTeamId GetGenericTeamId() const override;
	AGameModeOubliette* gm;
	float BaseSpellDamage;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	UFUNCTION(BlueprintCallable, Category = "Character | Utility")
	FLineTraceData tryLineTrace(float traceLength, USceneComponent* startComp);

	UFUNCTION(BlueprintCallable, Category = "Character | Stats")
	void calculateStats();

	UFUNCTION(BlueprintCallable, Category = "Character | Spells")
	void calcBaseDamage();
	UFUNCTION(BlueprintCallable, Category = "Character | Spells")
	FSpellDamageCalc calcSpellDamage();


	//Gameplay variables
	UPROPERTY(BlueprintReadWrite, Category = "Character | Gameplay")
	int32 HealthCurrent;
	UPROPERTY(BlueprintReadWrite, Category = "Character | Gameplay")
	int32 HealthMax;
	UPROPERTY(BlueprintReadWrite, Category = "Character | Gameplay")
	bool bIsDead;
	UPROPERTY(BlueprintReadWrite, Category = "Character | Gameplay")
	bool bInputEnabled;

	//Spell vars
	UPROPERTY(BlueprintReadWrite, Category = "Character | Spells")
	bool bIsAttackingL;
	UPROPERTY(BlueprintReadWrite, Category = "Character | Spells")
	bool bIsAttackingR;
	UPROPERTY(BlueprintReadWrite, Category = "Character | Spells")
	bool bCanAttackL = true;
	UPROPERTY(BlueprintReadWrite, Category = "Character | Spells")
	bool bCanAttackR = true;
	UPROPERTY(BlueprintReadWrite, Category = "Character | Spells")
	float ManaCurrent;
	UPROPERTY(BlueprintReadWrite, Category = "Character | Spells")
	float manaMax;
	UPROPERTY(BlueprintReadWrite, Category = "Character | Spells")
	float manaRecharge;

	UPROPERTY(BlueprintReadWrite, Category = "Character | Spells")
	EHandEnum HandEnumNEW = EHandEnum::HE_Left;
	UPROPERTY(BlueprintReadWrite, Category = "Character | Spells")
	FOffensiveSpellStruct ActiveSpellRNew;
	UPROPERTY(BlueprintReadWrite, Category = "Character | Spells")
	ESpellUtilsEnum ActiveSpellLNew;

	//Items
	UPROPERTY(BlueprintReadWrite, Category = "Character | Inventory")
	TArray<FItemStruct> Inventory;

	//Base Stats
	int32 baseInte = 1;
	int32 baseAgil = 1;
	int32 baseWisd = 1;
	int32 baseBonFire = 0;
	int32 baseBonFrost = 0;
	int32 baseBonShock = 0;
	int32 baseBonArcane = 0;
	int32 baseBonShadow = 0;
	int32 baseBonUndead = 0;
	int32 baseBonSlime = 0;
	int32 baseBonBeast = 0;
	int32 baseBonXP = 0;
	int32 baseBonLoot = 0;

	//Actual Working Stats
	UPROPERTY(BlueprintReadWrite, Category = "Character | Stats")
	int32 Inte;
	UPROPERTY(BlueprintReadWrite, Category = "Character | Stats")
	int32 Agil;
	UPROPERTY(BlueprintReadWrite, Category = "Character | Stats")
	int32 Wisd;
	UPROPERTY(BlueprintReadWrite, Category = "Character | Stats")
	int32 BonFire;
	UPROPERTY(BlueprintReadWrite, Category = "Character | Stats")
	int32 BonFrost;
	UPROPERTY(BlueprintReadWrite, Category = "Character | Stats")
	int32 BonShock;
	UPROPERTY(BlueprintReadWrite, Category = "Character | Stats")
	int32 BonArcane;
	UPROPERTY(BlueprintReadWrite, Category = "Character | Stats")
	int32 BonShadow;
	UPROPERTY(BlueprintReadWrite, Category = "Character | Stats")
	int32 BonUndead;
	UPROPERTY(BlueprintReadWrite, Category = "Character | Stats")
	int32 BonSlime;
	UPROPERTY(BlueprintReadWrite, Category = "Character | Stats")
	int32 BonBeast;
	UPROPERTY(BlueprintReadWrite, Category = "Character | Stats")
	int32 BonXP;
	UPROPERTY(BlueprintReadWrite, Category = "Character | Stats")
	int32 BonLoot;
};
