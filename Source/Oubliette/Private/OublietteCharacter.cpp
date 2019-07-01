// Fill out your copyright notice in the Description page of Project Settings.

#include "OublietteCharacter.h"
#include "Engine/World.h"

// Sets default values
AOublietteCharacter::AOublietteCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	TeamId = FGenericTeamId(0);

}

// Called when the game starts or when spawned
void AOublietteCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AOublietteCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AOublietteCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

FGenericTeamId AOublietteCharacter::GetGenericTeamId() const
{
	return TeamId;
}

//attempt a single line trace from a given component
FLineTraceData AOublietteCharacter::tryLineTrace(float traceLength, USceneComponent* startComp)
{
	TArray<AActor*> ignoredActors;
	FCollisionQueryParams RV_TraceParams;
	FVector tStart = startComp->GetComponentLocation();
	FVector tEnd = tStart + (startComp->GetForwardVector() * traceLength);

	//get game mode
	AGameModeOubliette* gm = (AGameModeOubliette*)GetWorld()->GetAuthGameMode();

	//ignore all room objects as well as self
	ignoredActors.Append(gm->allRooms);

	//setup trace parameters
	RV_TraceParams = FCollisionQueryParams(FName(TEXT("RV_Trace")), true, this);
	RV_TraceParams.bTraceComplex = true;
	RV_TraceParams.bReturnPhysicalMaterial = false;

	//Re-initialize hit info
	FHitResult RV_Hit(ForceInit);

	//call GetWorld() from within an actor extending class
	GetWorld()->LineTraceSingleByChannel(
		RV_Hit,			//result
		tStart,			//start
		tEnd,			//end
		ECC_Pawn,		//collision channel
		RV_TraceParams
	);

	FLineTraceData tData;

	tData.Location = RV_Hit.Location;
	tData.Return_Value = RV_Hit.bBlockingHit;
	tData.TraceEnd = RV_Hit.TraceEnd;
	tData.TraceStart = RV_Hit.TraceStart;
	tData.Distance = RV_Hit.Distance;
	tData.HitActor = RV_Hit.Actor;
	
	return tData;
}