// Fill out your copyright notice in the Description page of Project Settings.


#include "OublietteDoor.h"

// Sets default values
AOublietteDoor::AOublietteDoor(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	DoorMesh = ObjectInitializer.CreateDefaultSubobject<UStaticMeshComponent>(this, TEXT("DoorMesh"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> DoorMeshObj(TEXT("StaticMesh'/Game/Meshes/Static/SM_DoorNew.SM_DoorNew'"));
	DoorMesh->SetupAttachment(RootComponent);
	DoorMesh->SetStaticMesh(Cast<UStaticMesh>(DoorMeshObj.Object));
	DoorMesh->SetRelativeLocation(FVector(0.0f, 0.0f, 160.0f));

}

// Called when the game starts or when spawned
void AOublietteDoor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AOublietteDoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AOublietteDoor::Init(const FRotator newRotation, const FVector targetOffset)
{
	if (linkedDoor != nullptr)
	{
		targetLocation = linkedDoor->GetActorLocation() + targetOffset;
	}

	SetActorRotation(newRotation);
}

bool AOublietteDoor::UpdateSelection_Implementation(bool IsSelectedNew)
{
	return true;
}

void AOublietteDoor::Interact_Implementation()
{
}

