// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "HKAbilityTask_Dead.generated.h"

/**
 * 
 */
UCLASS()
class UNREALPORTFOLIO_API UHKAbilityTask_Dead : public UAbilityTask
{
	GENERATED_BODY()
	
public:
	UHKAbilityTask_Dead();

	static UHKAbilityTask_Dead* CreateTask(UGameplayAbility* OwningAbility);

	virtual void Activate() override;
	virtual void OnDestroy(bool AbilityEnded) override;

};
