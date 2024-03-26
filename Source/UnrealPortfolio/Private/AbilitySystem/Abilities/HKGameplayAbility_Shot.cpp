// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/HKGameplayAbility_Shot.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Characters/HKPlayerCharacter.h"
#include "AbilitySystem/AbilityTask/HKAbilityTask_Shot.h"
#include "Controllers/HKPlayerControllerBase.h"
#include "Item/HKWeapon.h"

UHKGameplayAbility_Shot::UHKGameplayAbility_Shot()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;

	ShotDelay = 0.2f;
}

void UHKGameplayAbility_Shot::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	AHKPlayerCharacter* Characrter = CastChecked<AHKPlayerCharacter>(ActorInfo->AvatarActor.Get());

	FName SectionName = *FString::Printf(TEXT("Default"));
	UAbilityTask_PlayMontageAndWait* PlayAttackTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, TEXT("PlayAttack"), Characrter->GetShotMontage(), 1.0f, SectionName);
	PlayAttackTask->OnCompleted.AddDynamic(this, &UHKGameplayAbility_Shot::OnCompleteCallback);
	PlayAttackTask->OnInterrupted.AddDynamic(this, &UHKGameplayAbility_Shot::OnInterruptedCallback);
	PlayAttackTask->ReadyForActivation();

	AHKPlayerControllerBase* ControllerBase = CastChecked<AHKPlayerControllerBase>(Characrter->GetController());
	ControllerBase->CameraShake(Characrter->GetWeapon()->GetRecoil());
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

