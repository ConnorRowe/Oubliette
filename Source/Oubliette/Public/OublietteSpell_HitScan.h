// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h" 
#include "OublietteSpell.h"
#include "OublietteSpell_HitScan.generated.h"

/**
 *
 */
UCLASS()
class OUBLIETTE_API AOublietteSpell_HitScan : public AOublietteSpell
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Spells")
		void initHitScan(float CalcedDamage, TSubclassOf<UDamageType> Element, float DoubleExplodeChance);
	virtual void initHitScan_Implementation(float CalcedDamage, TSubclassOf<UDamageType> Element, float DoubleExplodeChance);

	UFUNCTION(BlueprintCallable, Category = "Spells")
		void dealDamage();

	UPROPERTY(BlueprintReadWrite, Category = "Spells")
		float doubleExplodeChance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Assets")
		USoundCue* SpellExplosionCue;

};
