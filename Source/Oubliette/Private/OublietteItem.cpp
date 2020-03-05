// Fill out your copyright notice in the Description page of Project Settings.


#include "OublietteItem.h"

// Sets default values
AOublietteItem::AOublietteItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AOublietteItem::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AOublietteItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

bool AOublietteItem::UpdateSelection_Implementation(bool IsSelectedNew)
{
	return false;
}

void AOublietteItem::Interact_Implementation()
{
}

