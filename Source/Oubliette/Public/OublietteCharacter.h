// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Oubliette.h"
#include "GameModeOubliette.h"
#include "CollisionQueryParams.h"
#include "GameFramework/Character.h"
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
};
