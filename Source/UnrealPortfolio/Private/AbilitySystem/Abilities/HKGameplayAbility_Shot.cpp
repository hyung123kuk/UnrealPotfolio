// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/HKGameplayAbility_Shot.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Characters/HKPlayerCharacter.h"
#include "AbilitySystem/AbilityTask/HKAbilityTask_Shot.h"
#include "AbilitySystem/Abilities/HKGameplayAbility_Reload.h"
#include "Controllers/HKPlayerControllerBase.h"
#include "Item/HKWeapon.h"
#include "AbilitySystemComponent.h"

UHKGameplayAbility_Shot::UHKGameplayAbility_Shot()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;
}

bool UHKGameplayAbility_Shot::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, OUT FGameplayTagContainer* OptionalRelevantTags) const
{
	bool CanAbility = Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags);
	if (!CanAbility)
	{
		return false;
	}

	AHKPlayerCharacter* Character = CastChecked<AHKPlayerCharacter>(ActorInfo->AvatarActor.Get());
	AHKWeapon* Weapon = Character->GetWeapon();
	CanAbility = Weapon->LoadedBullet();
	if (!CanAbility)
	{
		UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo();
		TArray<FGameplayAbilitySpec>& AbilitySpecs = ASC->GetActivatableAbilities();

		//ReloadAbility ½ÇÇà
		for (FGameplayAbilitySpec& AbilitySpec : AbilitySpecs)
		{
			UHKGameplayAbility_Reload* ReloadAbility = Cast<UHKGameplayAbility_Reload>(AbilitySpec.Ability);
			if (ReloadAbility)
			{
				ASC->TryActivateAbility(AbilitySpec.Handle);
			}
		}

		FGameplayAbilitySpec* AbilitySpec = ASC->FindAbilitySpecFromClass(UHKGameplayAbility_Reload::StaticClass());
		
	}

	return CanAbility;
}

void UHKGameplayAbility_Shot::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	AHKPlayerCharacter* Character = CastChecked<AHKPlayerCharacter>(ActorInfo->AvatarActor.Get());

	FName SectionName = *FString::Printf(TEXT("Default"));
	UAbilityTask_PlayMontageAndWait* PlayAttackTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, TEXT("PlayAttack"), Character->GetShotMontage(), 1.0f, SectionName);
	PlayAttackTask->OnCompleted.AddDynamic(this, &UHKGameplayAbility_Shot::OnCompleteCallback);
	PlayAttackTask->OnInterrupted.AddDynamic(this, &UHKGameplayAbility_Shot::OnInterruptedCallback);
	PlayAttackTask->ReadyForActivation();

	AHKPlayerControllerBase* ControllerBase = CastChecked<AHKPlayerControllerBase>(Character->GetController());
	ControllerBase->CameraShake(Character->GetWeapon()->GetRecoil());
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

