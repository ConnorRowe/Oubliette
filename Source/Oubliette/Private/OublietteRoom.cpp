// Fill out your copyright notice in the Description page of Project Settings.
// thank you mojobojo from the ue forums

#include "OublietteRoom.h"

// Sets default values
AOublietteRoom::AOublietteRoom(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	USceneComponent* Root = ObjectInitializer.CreateDefaultSubobject<USceneComponent>(this, TEXT("Root"));
	this->SetRootComponent(Root);

	FloorMesh = ObjectInitializer.CreateDefaultSubobject<UStaticMeshComponent>(this, TEXT("FloorMesh"));
	RoomBoxVolume = ObjectInitializer.CreateDefaultSubobject<UBoxComponent>(this, TEXT("RoomBoxVolume"));

	FloorMesh->SetupAttachment(this->GetRootComponent());
	RoomBoxVolume->SetupAttachment(this->GetRootComponent());

	static ConstructorHelpers::FObjectFinder<UStaticMesh> FloorMeshObj(TEXT("StaticMesh'/Game/Meshes/Static/SM_1x1x10_Cuboid.SM_1x1x10_Cuboid'"));
	FloorMesh->SetStaticMesh(Cast<UStaticMesh>(FloorMeshObj.Object));

	static ConstructorHelpers::FObjectFinder<UMaterialInterface> FloorMatObj(TEXT("Material'/Game/Materials/M_Floor.M_Floor'"));
	FloorMesh->SetMaterial(0, Cast<UMaterialInterface>(FloorMatObj.Object));

}

// Called when the game starts or when spawned
void AOublietteRoom::BeginPlay()
{
	Super::BeginPlay();

	RoomBoxVolume->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);

}
