// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "OublietteTrapdoor.generated.h"

UCLASS()
class OUBLIETTE_API AOublietteTrapdoor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AOublietteTrapdoor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
