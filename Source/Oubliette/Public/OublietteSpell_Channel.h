// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "OublietteSpell.h"
#include "OublietteSpell_Channel.generated.h"

/**
 * 
 */
UCLASS()
class OUBLIETTE_API AOublietteSpell_Channel : public AOublietteSpell
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Spells")
	void initChannel(float CalcedDamage, TSubclassOf<UDamageType> Element, float TickRate, float DoubleExplodeChance);
	virtual void initChannel_Implementation(float CalcedDamage, TSubclassOf<UDamageType> Element, float TickRate, float DoubleExplodeChance);

	UPROPERTY(BlueprintReadWrite, Category = "Spells")
	float tickRate;
	UPROPERTY(BlueprintReadWrite, Category = "Spells")
	float doubleExplodeChance;
};
