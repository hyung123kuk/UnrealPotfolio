// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayModMagnitudeCalculation.h"
#include "HKMMC_Ammo.generated.h"

/**
 * 
 */
UCLASS()
class UNREALPORTFOLIO_API UHKMMC_Ammo : public UGameplayModMagnitudeCalculation
{
	GENERATED_BODY()
public:
	UHKMMC_Ammo();

	virtual float CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const override;

};
