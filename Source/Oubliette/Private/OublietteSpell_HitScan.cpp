// Fill out your copyright notice in the Description page of Project Settings.


#include "OublietteSpell_HitScan.h"

constexpr float BASE_DMG_RADIUS = 30.0f;

void AOublietteSpell_HitScan::initHitScan_Implementation(float CalcedDamage, TSubclassOf<UDamageType> Element, float DoubleExplodeChance)
{
	init(CalcedDamage, Element);
	doubleExplodeChance = DoubleExplodeChance;
}

void AOublietteSpell_HitScan::dealDamage()
{
	const TArray<AActor*> ignoredActors = TArray<AActor*>({ Cast<AActor>(GetInstigator()) });

	UGameplayStatics::ApplyRadialDamage(this, spellDamage, this->GetActorLocation(), (FMath::FRand() < doubleExplodeChance) ? BASE_DMG_RADIUS : BASE_DMG_RADIUS * 2, damageType, ignoredActors, Cast<AActor>(GetInstigator()));

	UGameplayStatics::PlaySoundAtLocation(this, SpellExplosionCue, this->GetActorLocation());
}
