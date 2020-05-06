// Fill out your copyright notice in the Description page of Project Settings.


#include "OublietteSpell_Projectile.h"

AOublietteSpell_Projectile::AOublietteSpell_Projectile(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	collisionComponent = ObjectInitializer.CreateDefaultSubobject<USphereComponent>(this, TEXT("CollisionComponent"));
	this->SetRootComponent(collisionComponent);

	collisionComponent->SetRelativeTransform(FTransform(FRotator(0.0f), FVector(0.0f), FVector(1.0f)));
	collisionComponent->SetSphereRadius(19.9f);
	collisionComponent->SetWalkableSlopeOverride(FWalkableSlopeOverride(EWalkableSlopeBehavior::WalkableSlope_Unwalkable, 0.0f));

	collisionComponent->SetNotifyRigidBodyCollision(true);
	collisionComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	collisionComponent->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel1);
	collisionComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel1, ECollisionResponse::ECR_Ignore);
	collisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

	SM_ExplosionDisplay->SetupAttachment(collisionComponent);

}

void AOublietteSpell_Projectile::initProjectile_Implementation(float CalcedDamage, TSubclassOf<UDamageType> Element, float ProjectileSpeed, float DoubleExplodeChance, float ProjectilePower)
{
	init(CalcedDamage, Element);
	projectileSpeed = ProjectileSpeed;
	doubleExplodeChance = DoubleExplodeChance;

	//Calculates if it is double radius or not
	float baseRadius = 30.0f;
	damageRadius = (FMath::FRand() < doubleExplodeChance) ? baseRadius : baseRadius * 2.0f;

	//Apply ProjectilePower visual scaling effect
	this->SetActorScale3D(FVector(ProjectilePower));

	//applies damage radius to the display mesh
	SM_ExplosionDisplay->SetWorldScale3D(FVector(damageRadius / 100.0f));

}