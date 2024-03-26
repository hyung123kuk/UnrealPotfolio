// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/HKGameplayAbility_AttackHitCheck.h"
#include "AbilitySystem/AbilityTask/HKAbilityTask_Shot.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "UnrealPortfolio/UnrealPortfolio.h"
#include "AbilitySystemComponent.h"
#include "Characters/HKPlayerCharacter.h"
#include "Item/HKWeapon.h"

UHKGameplayAbility_AttackHitCheck::UHKGameplayAbility_AttackHitCheck()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;
}

void UHKGameplayAbility_AttackHitCheck::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	
	AHKPlayerCharacter* PlayerCharacter = Cast<AHKPlayerCharacter>(ActorInfo->AvatarActor.Get());
	UHKAbilityTask_Shot* AttackTraceTask = UHKAbilityTask_Shot::CreateTask(this, PlayerCharacter->GetWeapon()->GetTargetActor());
	AttackTraceTask->OnComplete.AddDynamic(this, &UHKGameplayAbility_AttackHitCheck::OnTraceResultCallback);
	AttackTraceTask->ReadyForActivation();
}

void UHKGameplayAbility_AttackHitCheck::OnTraceResultCallback(const FGameplayAbilityTargetDataHandle& TargetDataHandle)
{
	if (UAbilitySystemBlueprintLibrary::TargetDataHasHitResult(TargetDataHandle, 0))
	{
		int HitTargetCount = TargetDataHandle.Num();

		for (int i = 0; i < HitTargetCount; i++)
		{
			FHitResult HitResult = UAbilitySystemBlueprintLibrary::GetHitResultFromTargetData(TargetDataHandle, i);
			
			UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(HitResult.GetActor());
			FGameplayEffectContextHandle ContextHandle = TargetASC->MakeEffectContext();
			ContextHandle.AddInstigator(GetAvatarActorFromActorInfo(), GetAvatarActorFromActorInfo());
			const FGameplayEffectSpecHandle EffectSpecHandle = TargetASC->MakeOutgoingSpec(AttackDamageEffect, 1.0f, ContextHandle);

			if (EffectSpecHandle.IsValid())
			{
				FGameplayAbilityTargetDataHandle ApplyTargetHandle = UAbilitySystemBlueprintLibrary::AbilityTargetDataFromHitResult(HitResult);
				ApplyGameplayEffectSpecToTarget(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, EffectSpecHandle, ApplyTargetHandle);

				FGameplayEffectContextHandle CueContextHandle = UAbilitySystemBlueprintLibrary::GetEffectContext(EffectSpecHandle);
				CueContextHandle.AddHitResult(HitResult);
				FGameplayCueParameters CueParam;
				CueParam.EffectContext = CueContextHandle;
				TargetASC->ExecuteGameplayCue(HKTAG_GAMEPLAYCUE_CHARACTER_ATTACKHIT, CueParam);
			}
		}

	}

	bool bReplicatedEndAbility = true;
	bool bWasCancelled = false;
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicatedEndAbility, bWasCancelled);
}
