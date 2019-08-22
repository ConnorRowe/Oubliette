// Fill out your copyright notice in the Description page of Project Settings.


#include "OublietteSpell_HitScan.h"

void AOublietteSpell_HitScan::initHitScan_Implementation(float CalcedDamage, TSubclassOf<UDamageType> Element, float DoubleExplodeChance)
{
	init(CalcedDamage, Element);
	doubleExplodeChance = DoubleExplodeChance;
}