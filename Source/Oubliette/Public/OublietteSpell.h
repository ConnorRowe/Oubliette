// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h" 
#include "OublietteSpell.generated.h"

UCLASS()
class OUBLIETTE_API AOublietteSpell : public AActor
{
	GENERATED_BODY()

public:
	AOublietteSpell(const FObjectInitializer& ObjectInitializer);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "Spells")
		void init(float Damage, TSubclassOf<UDamageType> DamageType);

	UPROPERTY(BlueprintReadWrite, Category = "Spells")
		float spellDamage;
	UPROPERTY(BlueprintReadWrite, Category = "Spells")
		TSubclassOf<UDamageType> damageType;
	UPROPERTY(BlueprintReadWrite, Category = "Spells")
		float damageRadius;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		UStaticMeshComponent* SM_ExplosionDisplay;
};
