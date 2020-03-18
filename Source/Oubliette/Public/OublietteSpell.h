// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "OublietteSpell.generated.h"

UCLASS()
class OUBLIETTE_API AOublietteSpell : public AActor
{
	GENERATED_BODY()

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
};
