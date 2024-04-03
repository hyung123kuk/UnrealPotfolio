// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/HKGameplayAbility_WeaponSwap.h"
#include "Characters/HKCharacterBase.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Item/HKWeapon.h"
#include "AbilitySystemComponent.h"
#include "Characters/HKPlayerCharacter.h"

UHKGameplayAbility_WeaponSwap::UHKGameplayAbility_WeaponSwap()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;
}

bool UHKGameplayAbility_WeaponSwap::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, OUT FGameplayTagContainer* OptionalRelevantTags) const
{
	if (!Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags))
	{
		return false;
	}

	AHKPlayerCharacter* PlayerCharacter = Cast<AHKPlayerCharacter>(ActorInfo->AvatarActor.Get());
	if (PlayerCharacter->GetWeapon() == SwapWeapon.GetDefaultObject())
	{
		return false;
	}
	return true;
}

void UHKGameplayAbility_WeaponSwap::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	AHKPlayerCharacter* PlayerCharacter = Cast<AHKPlayerCharacter>(ActorInfo->AvatarActor.Get());

	FName SectionName = *FString::Printf(TEXT("Default"));

	UAbilityTask_PlayMontageAndWait* SwapMontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, TEXT("PlaySwap"), PlayerCharacter->GetSwapMontage(), 1.0f, SectionName);
	SwapMontageTask->OnCompleted.AddDynamic(this, &UHKGameplayAbility_WeaponSwap::OnCompleteCallback);
	SwapMontageTask->OnInterrupted.AddDynamic(this, &UHKGameplayAbility_WeaponSwap::OnInterruptedCallback);
	SwapMontageTask->ReadyForActivation();

	PlayerCharacter->SetWeapon(SwapWeapon.GetDefaultObject());

}

void UHKGameplayAbility_WeaponSwap::OnCompleteCallback()
{
	bool bReplicatedEndAbility = true;
	bool bWasCancelled = false;
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicatedEndAbility, bWasCancelled);
}

void UHKGameplayAbility_WeaponSwap::OnInterruptedCallback()
{
	bool bReplicatedEndAbility = true;
	bool bWasCancelled = true;
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicatedEndAbility, bWasCancelled);
}