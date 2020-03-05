// Fill out your copyright notice in the Description page of Project Settings.


#include "OublietteChest.h"

// Sets default values
AOublietteChest::AOublietteChest()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AOublietteChest::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AOublietteChest::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

bool AOublietteChest::UpdateSelection_Implementation(bool IsSelectedNew)
{
	return false;
}

void AOublietteChest::Interact_Implementation()
{
}

