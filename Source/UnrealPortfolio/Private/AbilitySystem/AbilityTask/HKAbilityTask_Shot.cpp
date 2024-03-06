// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/AbilityTask/HKAbilityTask_Shot.h"
#include "AbilitySystem/AbilityTask/TargetActor/HKTargetActor_Shot.h"
#include "AbilitySystemComponent.h"

UHKAbilityTask_Shot::UHKAbilityTask_Shot()
{

}

UHKAbilityTask_Shot* UHKAbilityTask_Shot::CreateTask(UGameplayAbility* OwningAbility, TSubclassOf<class AHKTargetActor_Shot> TargetActorClass)
{
	UHKAbilityTask_Shot* NewTask = NewAbilityTask<UHKAbilityTask_Shot>(OwningAbility);
	NewTask->TargetActorClass = TargetActorClass;
	return NewTask;
}

void UHKAbilityTask_Shot::Activate()
{
	Super::Activate();

	SpawnedTargetActor = Cast<AHKTargetActor_Shot>(Ability->GetWorld()->SpawnActorDeferred<AGameplayAbilityTargetActor>(TargetActorClass, FTransform::Identity, nullptr, nullptr, ESpawnActorCollisionHandlingMethod::AlwaysSpawn));
	if (SpawnedTargetActor)
	{
		SpawnedTargetActor->TargetDataReadyDelegate.AddUObject(this, &UHKAbilityTask_Shot::OnTargetDataReadyCallback);
	}

	UAbilitySystemComponent* ASC = AbilitySystemComponent.Get();

	if (ASC)
	{
		const FTransform SpawnTransform = ASC->GetAvatarActor()->GetTransform();
		SpawnedTargetActor->FinishSpawning(SpawnTransform);

		ASC->SpawnedTargetActors.Push(SpawnedTargetActor);
		SpawnedTargetActor->StartTargeting(Ability);
		SpawnedTargetActor->ConfirmTargeting();
	}

	SetWaitingOnAvatar();
}

void UHKAbilityTask_Shot::OnDestroy(bool AbilityEnded)
{
	if (SpawnedTargetActor)
	{
		SpawnedTargetActor->Destroy();
	}

	Super::OnDestroy(AbilityEnded);
}

void UHKAbilityTask_Shot::OnTargetDataReadyCallback(const FGameplayAbilityTargetDataHandle& DataHandle)
{
	if (ShouldBroadcastAbilityTaskDelegates())
	{
		OnComplete.Broadcast(DataHandle);
	}

	EndTask();
}
