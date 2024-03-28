// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/HKGameplayAbility_Dead.h"
#include "Characters/HKCharacterBase.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Engine/EngineTypes.h"
#include "Engine/DamageEvents.h"
#include "AbilitySystem/AttributeSet/HKCharacterAttributeSet.h"
#include "AbilitySystem/AbilityTask/HKAbilityTask_Dead.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"


UHKGameplayAbility_Dead::UHKGameplayAbility_Dead()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::ServerInitiated;
}

bool UHKGameplayAbility_Dead::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, OUT FGameplayTagContainer* OptionalRelevantTags) const
{
	bool bResult = Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags);
	if (!bResult)
	{
		return false;
	}

	return true;
}

void UHKGameplayAbility_Dead::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	AHKCharacterBase* Character = CastChecked<AHKCharacterBase>(ActorInfo->AvatarActor.Get());

	UHKAbilityTask_Dead* DeadTask = UHKAbilityTask_Dead::CreateTask(this);
	DeadTask->ReadyForActivation();

	FName SectionName = *FString::Printf(TEXT("Default"));
	UAbilityTask_PlayMontageAndWait* DeadMontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, TEXT("PlayDead"), Character->GetDeadMontage(), 1.0f, SectionName);
	DeadMontageTask->ReadyForActivation();
	if (Character)
	{
		Character->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		Character->GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		Character->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
	}

}


