// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/GameplayEffect/ModMagnitudeCalculation/HKMMC_Ammo.h"
#include "Characters/HKPlayerCharacter.h"
#include "Item/HKWeapon.h"

UHKMMC_Ammo::UHKMMC_Ammo()
{
}

float UHKMMC_Ammo::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	AHKPlayerCharacter* Character = Cast<AHKPlayerCharacter>(Spec.GetContext().GetEffectCauser());

	Character->GetWeapon()->AddAmmo();

	return 0.0f;
}
