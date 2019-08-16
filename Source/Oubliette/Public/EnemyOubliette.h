// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EnemyOubliette.generated.h"

UCLASS()
class OUBLIETTE_API AEnemyOubliette : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemyOubliette();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//Status Effect Booleans
	UPROPERTY(BlueprintReadWrite, Category = "Enemy")
	bool isBleeding;
	UPROPERTY(BlueprintReadWrite, Category = "Enemy")
	bool isChilled;
	UPROPERTY(BlueprintReadWrite, Category = "Enemy")
	bool isBurning;
	UPROPERTY(BlueprintReadWrite, Category = "Enemy")
	bool isStunned;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
