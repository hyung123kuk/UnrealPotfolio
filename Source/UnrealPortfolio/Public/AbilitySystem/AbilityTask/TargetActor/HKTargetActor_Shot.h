// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbilityTargetActor.h"
#include "HKTargetActor_Shot.generated.h"

/**
 * 
 */
UCLASS()
class UNREALPORTFOLIO_API AHKTargetActor_Shot : public AGameplayAbilityTargetActor
{
	GENERATED_BODY()
	
public:
	AHKTargetActor_Shot();
	virtual void StartTargeting(UGameplayAbility* Ability) override;
	virtual void ConfirmTargetingAndContinue() override;

protected:
	virtual FGameplayAbilityTargetDataHandle MakeTargetData() const;
};
