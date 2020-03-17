// Fill out your copyright notice in the Description page of Project Settings.


#include "OublietteSpell_Channel.h"

AOublietteSpell_Channel::AOublietteSpell_Channel(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	damageNiagara = ObjectInitializer.CreateDefaultSubobject<UNiagaraComponent>(this, TEXT("NS_Damage"));

	SetRootComponent(damageNiagara);

}

void AOublietteSpell_Channel::initChannel_Implementation(float CalcedDamage, TSubclassOf<UDamageType> Element, float TickRate, float DoubleExplodeChance, FLinearColor ParticleColour, UNiagaraSystem* DamageNiagara)
{
	init(CalcedDamage, Element);
	tickRate = TickRate;
	doubleExplodeChance = DoubleExplodeChance;
	damageNiagara->SetAsset(DamageNiagara);
	damageNiagara->Activate();
	damageNiagara->SetVariableLinearColor(TEXT("User.ParticleColour"), ParticleColour);

}

void AOublietteSpell_Channel::finish_Implementation()
{
}
