// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/AttributeSet/HKCharacterAttributeSet.h"

UHKCharacterAttributeSet::UHKCharacterAttributeSet()
	: MaxHealth(100.f),
	Damage(0.f)
{
	InitHealth(GetMaxHealth());
}
