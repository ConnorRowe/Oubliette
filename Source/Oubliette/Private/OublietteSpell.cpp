// Fill out your copyright notice in the Description page of Project Settings.


#include "OublietteSpell.h"

void AOublietteSpell::init(float Damage, TSubclassOf<UDamageType> DamageType)
{
	spellDamage = Damage;
	damageType = DamageType;

}


// Called when the game starts or when spawned
void AOublietteSpell::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AOublietteSpell::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

