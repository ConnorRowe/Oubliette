// Fill out your copyright notice in the Description page of Project Settings.


#include "OublietteSpell.h"

// Sets default values
AOublietteSpell::AOublietteSpell()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

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

