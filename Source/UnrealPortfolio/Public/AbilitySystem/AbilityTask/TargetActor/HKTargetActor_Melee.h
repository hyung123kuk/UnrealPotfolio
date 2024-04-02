// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/AbilityTask/TargetActor/HKTargetActorBase.h"
#include "HKTargetActor_Melee.generated.h"

/**
 * 
 */
UCLASS()
class UNREALPORTFOLIO_API AHKTargetActor_Melee : public AHKTargetActorBase
{
	GENERATED_BODY()
	
public:
	AHKTargetActor_Melee();
	virtual void StartTargeting(UGameplayAbility* Ability) override;

protected:
	virtual FGameplayAbilityTargetDataHandle MakeTargetData() const override;

	float AttackRadius = 50.f;

};
