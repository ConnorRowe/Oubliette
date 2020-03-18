// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyOubliette.h"

// Sets default values
AEnemyOubliette::AEnemyOubliette()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AEnemyOubliette::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AEnemyOubliette::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AEnemyOubliette::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

