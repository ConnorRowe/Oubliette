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

void AOublietteDoor::Init_Implementation(const FRotator newRotation, const FVector targetOffset)
{
	FName paramName = FName(TEXT("GlowColour"));
	FVector doorColour = FVector();

	if (linkedDoor != nullptr)
	{
		targetLocation = linkedDoor->GetActorLocation() + targetOffset;

		switch (linkedDoor->RoomType)
		{
		case ERoomTypeEnum::ERT_Boss:
			doorColour = FVector(1.0f, 0.0f, 0.0f);
			break;
		case ERoomTypeEnum::ERT_Treasure:
			doorColour = FVector(1.0f, 0.866667f, 0.0f);
			break;
		default:
			doorColour = FVector(0.850001f, 0.0f, 1.0f);
			break;
		}
	}

	this->SetActorRotation(newRotation);
	this->DoorColour = doorColour;

	DoorMesh->SetVectorParameterValueOnMaterials(paramName, doorColour * 3.0f);
}

bool AOublietteDoor::UpdateSelection_Implementation(bool IsSelectedNew)
{
	return true;
}

void AOublietteDoor::Interact_Implementation()
{
}

