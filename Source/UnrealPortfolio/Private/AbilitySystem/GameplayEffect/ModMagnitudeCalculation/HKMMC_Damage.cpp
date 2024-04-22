// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/GameplayEffect/ModMagnitudeCalculation/HKMMC_Damage.h"
#include "Characters/HKPlayerCharacter.h"
#include "AbilitySystemComponent.h"
#include "Item/HKWeapon.h"
#include "Kismet/GameplayStatics.h"
#include "Game/HKGameState.h"

UHKMMC_Damage::UHKMMC_Damage()
{
}

float UHKMMC_Damage::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{

	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;

	AHKPlayerCharacter* Character = Cast<AHKPlayerCharacter>(Spec.GetContext().GetEffectCauser());
	
	float Damage = Character->GetWeapon()->GetDamage();

	if (Character->GetAbilitySystemComponent()->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag(FName("Character.Buff.DamageUp"))))
	{
		Damage *= 2;
	}

	return Damage;
}
