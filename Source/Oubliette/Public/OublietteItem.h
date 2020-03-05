// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Oubliette.h"
#include "GameFramework/Actor.h"
#include "Selectable.h"
#include "Interactable.h"
#include "OublietteItem.generated.h"

UCLASS()
class OUBLIETTE_API AOublietteItem : public AActor, public ISelectable, public IInteractable
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AOublietteItem();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(BlueprintReadWrite, Category = "Item")
	FItemStruct ItemData;

	//Selectable interface
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		bool UpdateSelection(bool IsSelectedNew);
	virtual bool UpdateSelection_Implementation(bool IsSelectedNew) override;

	//Interactable interface
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		void Interact();
	virtual void Interact_Implementation() override;
};
