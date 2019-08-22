// Fill out your copyright notice in the Description page of Project Settings.


#include "OublietteSpell_Projectile.h"

void AOublietteSpell_Projectile::initProjectile_Implementation(float CalcedDamage, TSubclassOf<UDamageType> Element, float ProjectileSpeed, float DoubleExplodeChance)
{
	init(CalcedDamage, Element);
	projectileSpeed = ProjectileSpeed;
	doubleExplodeChance = DoubleExplodeChance;
}