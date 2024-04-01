// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/AbilityTask/TargetActor/HKTargetActorBase.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Characters/HKPlayerCharacter.h"
#include "Camera/CameraComponent.h"

AHKTargetActorBase::AHKTargetActorBase()
{
}

void AHKTargetActorBase::InitTargetActor(UGameplayAbility* Ability)
{
	SourceActor = Ability->GetCurrentActorInfo()->AvatarActor.Get();
	PlayerCharacter = CastChecked<AHKPlayerCharacter>(SourceActor);
	CameraComponent = PlayerCharacter->GetCameraComponent();

	Weapon = PlayerCharacter->GetWeapon();
	MuzzleLocation = PlayerCharacter->GetMesh()->GetSocketLocation("MuzzleFlash");
	MuzzleRotation = PlayerCharacter->GetControlRotation();;
	CameraLocation = CameraComponent->GetComponentLocation();
}

void AHKTargetActorBase::StartTargeting(UGameplayAbility* Ability)
{
	Super::StartTargeting(Ability);
}


void AHKTargetActorBase::ConfirmTargetingAndContinue()
{
	if (SourceActor)
	{
		FGameplayAbilityTargetDataHandle DataHandle = MakeTargetData();
		TargetDataReadyDelegate.Broadcast(DataHandle);
	}
}

FGameplayAbilityTargetDataHandle AHKTargetActorBase::MakeTargetData() const
{
	return FGameplayAbilityTargetDataHandle();
}
