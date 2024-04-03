// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/HKGameplayAbility_Zoom.h"
#include "Characters/HKPlayerCharacter.h"
#include "Item/HKWeapon.h"
#include "GameframeWork/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "AbilitySystemComponent.h"

UHKGameplayAbility_Zoom::UHKGameplayAbility_Zoom()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;
}

bool UHKGameplayAbility_Zoom::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, OUT FGameplayTagContainer* OptionalRelevantTags) const
{
	if (!Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags))
	{
		return false;
	}

	//이미 실행중일 때 줌 취소
	AHKPlayerCharacter* Character = CastChecked<AHKPlayerCharacter>(ActorInfo->AvatarActor.Get());
	if (Character->GetIsZoom())
	{
		Character->GetAbilitySystemComponent()->CancelAbilityHandle(Handle);
		return false;
	}

	// 무기의 Zoom 이전, 이후 FiedOfView값과 TargetArmLegth값이 동일하다면 Zoom 효과는 없음.
	AHKWeapon* Weapon = Character->GetWeapon();
	if (Weapon->GetBeforeZoomFieldOfView() == Weapon->GetAfterZoomFieldOfView() 
		&& Weapon->GetBeforeZoomTargetArmLength() == Weapon->GetAfterZoomTargetArmLength())
	{
		return false;
	}

	return true;
}

void UHKGameplayAbility_Zoom::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	AHKPlayerCharacter* Character = CastChecked<AHKPlayerCharacter>(ActorInfo->AvatarActor.Get());
	AHKWeapon* Weapon = Character->GetWeapon();
	Character->GetCameraComponent()->FieldOfView = Weapon->GetAfterZoomFieldOfView();
	Character->GetSpringArmComponent()->TargetArmLength = Weapon->GetAfterZoomTargetArmLength();
	Character->SetIsZoom(true);
}

void UHKGameplayAbility_Zoom::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	AHKPlayerCharacter* Character = CastChecked<AHKPlayerCharacter>(ActorInfo->AvatarActor.Get());
	AHKWeapon* Weapon = Character->GetWeapon();
	Character->GetCameraComponent()->FieldOfView = Weapon->GetBeforeZoomFieldOfView();
	Character->GetSpringArmComponent()->TargetArmLength = Weapon->GetBeforeZoomTargetArmLength();
	Character->SetIsZoom(false);

	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}
