// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/HKGameplayAbility_AttackHitCheck.h"
#include "AbilitySystem/AbilityTask/HKAbilityTask_Attack.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "UnrealPortfolio/UnrealPortfolio.h"
#include "AbilitySystemComponent.h"
#include "Characters/HKPlayerCharacter.h"
#include "Item/HKWeapon.h"
#include "Kismet/KismetSystemLibrary.h"

UHKGameplayAbility_AttackHitCheck::UHKGameplayAbility_AttackHitCheck()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;
}

void UHKGameplayAbility_AttackHitCheck::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	
	AHKPlayerCharacter* PlayerCharacter = Cast<AHKPlayerCharacter>(ActorInfo->AvatarActor.Get());
	UHKAbilityTask_Attack* AttackTraceTask = UHKAbilityTask_Attack::CreateTask(this, PlayerCharacter->GetWeapon()->GetTargetActor());
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
			
			float EffectLevel = ConfirmEffectLevelByBoneName(HitResult.BoneName.ToString());

			UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(HitResult.GetActor());
			FGameplayEffectContextHandle ContextHandle = TargetASC->MakeEffectContext();
			ContextHandle.AddInstigator(GetAvatarActorFromActorInfo(), GetAvatarActorFromActorInfo());
			const FGameplayEffectSpecHandle EffectSpecHandle = TargetASC->MakeOutgoingSpec(AttackDamageEffect, EffectLevel, ContextHandle);

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
	else if (UAbilitySystemBlueprintLibrary::TargetDataHasActor(TargetDataHandle, 0))
	{
		UAbilitySystemComponent* SourceASC = GetAbilitySystemComponentFromActorInfo_Checked();

		FGameplayEffectSpecHandle EffectSpecHandle = MakeOutgoingGameplayEffectSpec(AttackDamageEffect);
		if (EffectSpecHandle.IsValid())
		{
			ApplyGameplayEffectSpecToTarget(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, EffectSpecHandle, TargetDataHandle);

			FGameplayEffectContextHandle CueContextHandle = UAbilitySystemBlueprintLibrary::GetEffectContext(EffectSpecHandle);
			CueContextHandle.AddActors(TargetDataHandle.Data[0].Get()->GetActors(), false);
			FGameplayCueParameters CueParam;
			CueParam.EffectContext = CueContextHandle;

			SourceASC->ExecuteGameplayCue(HKTAG_GAMEPLAYCUE_CHARACTER_ATTACKHIT, CueParam);
		}

	}

	bool bReplicatedEndAbility = true;
	bool bWasCancelled = false;
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicatedEndAbility, bWasCancelled);
}

float UHKGameplayAbility_AttackHitCheck::ConfirmEffectLevelByBoneName(FString BoneNameString) const
{
	if (BoneNameString.Equals(FString(TEXT("HEAD")), ESearchCase::IgnoreCase))
	{
		return 2.f;
	}
	else if (BoneNameString.Contains(FString(TEXT("SPINE")),ESearchCase::IgnoreCase))
	{
		return 1.5f;
	}

	return 1.f;
}
