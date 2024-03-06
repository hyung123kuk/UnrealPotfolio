// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/AbilityTask/HKAbilityTask_Dead.h"
#include "Engine/EngineTypes.h"
#include "Engine/DamageEvents.h"
#include "AbilitySystem/AttributeSet/HKCharacterAttributeSet.h"
#include "AbilitySystemComponent.h"
#include "UnrealPortfolio/UnrealPortfolio.h"

UHKAbilityTask_Dead::UHKAbilityTask_Dead()
{
}

UHKAbilityTask_Dead* UHKAbilityTask_Dead::CreateTask(UGameplayAbility* OwningAbility)
{
	UHKAbilityTask_Dead* NewTask = NewAbilityTask<UHKAbilityTask_Dead>(OwningAbility);
	return NewTask;
}

void UHKAbilityTask_Dead::Activate()
{
	UAbilitySystemComponent* ASC = AbilitySystemComponent.Get();

	ASC->ExecuteGameplayCue(HKTAG_GAMEPLAYCUE_CHARACTER_DEAD);
	// TODO : ReGameLogic
}

void UHKAbilityTask_Dead::OnDestroy(bool AbilityEnded)
{
}
