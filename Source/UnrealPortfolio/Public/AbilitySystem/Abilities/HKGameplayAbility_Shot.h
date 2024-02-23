// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "HKGameplayAbility_Shot.generated.h"

/**
 * 
 */
UCLASS()
class UNREALPORTFOLIO_API UHKGameplayAbility_Shot : public UGameplayAbility
{
	GENERATED_BODY()
	
public:
	UHKGameplayAbility_Shot();
	
protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	
protected:
	UFUNCTION()
	void OnCompleteCallback();

	UFUNCTION()
	void OnInterruptedCallback();

protected:
	float ShotDelay;
	FTimerHandle ComboTimerHandle;
	
};
