// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CoreUObject/Public/Uobject/ConstructorHelpers.h"
#include "Engine/Classes/Components/StaticMeshComponent.h"
#include "Selectable.h"
#include "Interactable.h"
#include "Oubliette.h"
#include "OublietteDoor.generated.h"

UCLASS()
class OUBLIETTE_API AOublietteDoor : public AActor, public ISelectable, public IInteractable
{
	GENERATED_BODY()

public:
	AOublietteDoor(const FObjectInitializer& ObjectInitializer);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		void Init(const FRotator newRotation, const FVector targetOffset);

	UPROPERTY(BlueprintReadWrite)
		UStaticMeshComponent* DoorMesh;

	UPROPERTY(BlueprintReadWrite)
		FVector targetLocation = FVector(0.0f);

	UPROPERTY(BlueprintReadWrite)
		AOublietteDoor* linkedDoor;	
	
	UPROPERTY(BlueprintReadWrite)
		ERoomTypeEnum RoomType;

	UPROPERTY(BlueprintReadWrite)
		FVector DoorColour;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	bool UpdateSelection(bool IsSelectedNew);
		virtual bool UpdateSelection_Implementation(bool IsSelectedNew) override;

	//Interactable interface
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void Interact();
		virtual void Interact_Implementation() override;
};
