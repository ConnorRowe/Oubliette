// Fill out your copyright notice in the Description page of Project Settings.


#include "OublietteDoor.h"

// Sets default values
AOublietteDoor::AOublietteDoor(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	USceneComponent* origin = ObjectInitializer.CreateDefaultSubobject<USceneComponent>(this, TEXT("Root"));
	this->SetRootComponent(origin);

	DoorMesh = ObjectInitializer.CreateDefaultSubobject<UStaticMeshComponent>(this, TEXT("DoorMesh"));
	DoorMesh->SetupAttachment(origin);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> DoorMeshObj(TEXT("StaticMesh'/Game/Meshes/Static/SM_Portal.SM_Portal'"));
	DoorMesh->SetStaticMesh(Cast<UStaticMesh>(DoorMeshObj.Object));

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

	this->SetActorRotation(newRotation);

}

bool AOublietteDoor::UpdateSelection_Implementation(bool IsSelectedNew)
{
	return true;
}

void AOublietteDoor::Interact_Implementation()
{
}

