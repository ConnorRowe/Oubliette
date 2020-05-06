// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SphereComponent.h"
#include "OublietteSpell.h"
#include "OublietteSpell_Projectile.generated.h"

/**
 *
 */
UCLASS()
class OUBLIETTE_API AOublietteSpell_Projectile : public AOublietteSpell
{
	GENERATED_BODY()

public:
	AOublietteSpell_Projectile(const FObjectInitializer& ObjectInitializer);

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Spells")
		void initProjectile(float CalcedDamage, TSubclassOf<UDamageType> Element, float ProjectileSpeed, float DoubleExplodeChance, float ProjectilePower);
	virtual void initProjectile_Implementation(float CalcedDamage, TSubclassOf<UDamageType> Element, float ProjectileSpeed, float DoubleExplodeChance, float ProjectilePower);

	UPROPERTY(BlueprintReadWrite, Category = "Spells")
		float projectileSpeed;
	UPROPERTY(BlueprintReadWrite, Category = "Spells")
		float doubleExplodeChance;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		USphereComponent* collisionComponent;
};
