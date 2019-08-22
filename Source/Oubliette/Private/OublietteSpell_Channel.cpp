// Fill out your copyright notice in the Description page of Project Settings.


#include "OublietteSpell_Channel.h"

void AOublietteSpell_Channel::initChannel_Implementation(float CalcedDamage, TSubclassOf<UDamageType> Element, float TickRate, float DoubleExplodeChance)
{
	init(CalcedDamage, Element);
	tickRate = TickRate;
	doubleExplodeChance = DoubleExplodeChance;
}