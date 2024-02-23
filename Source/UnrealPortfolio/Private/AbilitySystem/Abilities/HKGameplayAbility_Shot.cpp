// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/HKGameplayAbility_Shot.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Characters/HKPlayerCharacter.h"

UHKGameplayAbility_Shot::UHKGameplayAbility_Shot()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;

	ShotDelay = 0.2f;
}

void UHKGameplayAbility_Shot::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	AHKCharacterBase* Characrter = CastChecked<AHKCharacterBase>(ActorInfo->AvatarActor.Get());

	FName SectionName = *FString::Printf(TEXT("Default"));
	UAbilityTask_PlayMontageAndWait* PlayAttackTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, TEXT("PlayAttack"), Characrter->GetShotMontage(), 1.0f, SectionName);
	PlayAttackTask->OnCompleted.AddDynamic(this, &UHKGameplayAbility_Shot::OnCompleteCallback);
	PlayAttackTask->OnInterrupted.AddDynamic(this, &UHKGameplayAbility_Shot::OnInterruptedCallback);
	PlayAttackTask->ReadyForActivation();
}

void UHKGameplayAbility_Shot::OnCompleteCallback()
{
	bool bReplicatedEndAbility = true;
	bool bWasCancelled = false;
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicatedEndAbility, bWasCancelled);
}

void UHKGameplayAbility_Shot::OnInterruptedCallback()
{
	bool bReplicatedEndAbility = true;
	bool bWasCancelled = true;
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicatedEndAbility, bWasCancelled);
}

