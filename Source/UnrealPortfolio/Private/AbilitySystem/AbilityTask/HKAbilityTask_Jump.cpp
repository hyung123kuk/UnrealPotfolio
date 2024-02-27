// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/AbilityTask/HKAbilityTask_Jump.h"
#include "GameFramework/Character.h"

UHKAbilityTask_Jump::UHKAbilityTask_Jump()
{
}

UHKAbilityTask_Jump* UHKAbilityTask_Jump::CreateTask(UGameplayAbility* OwningAbility)
{
	UHKAbilityTask_Jump* NewTask = NewAbilityTask<UHKAbilityTask_Jump>(OwningAbility);
	return NewTask;
}

void UHKAbilityTask_Jump::Activate()
{
	Super::Activate();

	ACharacter* Character = CastChecked<ACharacter>(GetAvatarActor());
	Character->LandedDelegate.AddDynamic(this, &UHKAbilityTask_Jump::OnLandedCallback);
	Character->Jump();
	SetWaitingOnAvatar();
}

void UHKAbilityTask_Jump::OnDestroy(bool AbilityEnded)
{
	ACharacter* Character = CastChecked<ACharacter>(GetAvatarActor());
	Character->LandedDelegate.RemoveDynamic(this, &UHKAbilityTask_Jump::OnLandedCallback);

	Super::OnDestroy(AbilityEnded);
}

void UHKAbilityTask_Jump::OnLandedCallback(const FHitResult& Hit)
{
	if (ShouldBroadcastAbilityTaskDelegates())
	{
		OnComplete.Broadcast();
	}
}

