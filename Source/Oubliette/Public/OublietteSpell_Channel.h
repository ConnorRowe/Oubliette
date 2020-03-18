// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "OublietteSpell.h"
#include "NiagaraComponent.h"
#include "NiagaraSystem.h"
#include "OublietteSpell_Channel.generated.h"

/**
 *
 */
UCLASS()
class OUBLIETTE_API AOublietteSpell_Channel : public AOublietteSpell
{
	GENERATED_BODY()

public:
	AOublietteSpell_Channel(const FObjectInitializer& ObjectInitializer);

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Spells")
		void initChannel(float CalcedDamage, TSubclassOf<UDamageType> Element, float TickRate, float DoubleExplodeChance, FLinearColor ParticleColour, UNiagaraSystem* DamageNiagara);
	virtual void initChannel_Implementation(float CalcedDamage, TSubclassOf<UDamageType> Element, float TickRate, float DoubleExplodeChance, FLinearColor ParticleColour, UNiagaraSystem* DamageNiagara);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Spells")
		void finish();
	virtual void finish_Implementation();

	UPROPERTY(BlueprintReadWrite, Category = "Spells")
		float tickRate;
	UPROPERTY(BlueprintReadWrite, Category = "Spells")
		float doubleExplodeChance;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		UNiagaraComponent* damageNiagara;
};
