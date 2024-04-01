// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbilityTargetActor.h"
#include "HKTargetActorBase.h"
#include "HKTargetActor_Shot.generated.h"

/**
 * 
 */
UCLASS()
class UNREALPORTFOLIO_API AHKTargetActor_Shot : public AHKTargetActorBase
{
	GENERATED_BODY()
	
public:
	AHKTargetActor_Shot();
	virtual void StartTargeting(UGameplayAbility* Ability) override;

protected:
	virtual FGameplayAbilityTargetDataHandle MakeTargetData() const override;

};
