// Fill out your copyright notice in the Description page of Project Settings.


#include "OublietteSpell_Projectile.h"

AOublietteSpell_Projectile::AOublietteSpell_Projectile(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	collisionComponent = ObjectInitializer.CreateDefaultSubobject<USphereComponent>(this, TEXT("CollisionComponent"));

	SetRootComponent(collisionComponent);

	collisionComponent->SetSphereRadius(19.9f);
	collisionComponent->SetWalkableSlopeOverride(FWalkableSlopeOverride(EWalkableSlopeBehavior::WalkableSlope_Unwalkable, 0.0f));

	collisionComponent->SetNotifyRigidBodyCollision(true);
	collisionComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	collisionComponent->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel1);
	collisionComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel1, ECollisionResponse::ECR_Ignore);
	collisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

}

void AOublietteSpell_Projectile::initProjectile_Implementation(float CalcedDamage, TSubclassOf<UDamageType> Element, float ProjectileSpeed, float DoubleExplodeChance)
{
	init(CalcedDamage, Element);
	projectileSpeed = ProjectileSpeed;
	doubleExplodeChance = DoubleExplodeChance;
}