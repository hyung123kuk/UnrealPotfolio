// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/AbilityTask/HKAbilityTask_Attack.h"
#include "AbilitySystem/AbilityTask/TargetActor/HKTargetActor_Shot.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Characters/HKPlayerCharacter.h"
#include "Camera/CameraComponent.h"
#include "AbilitySystem/AbilityTask/TargetActor/HKTargetActor_Projectile.h"
#include "Net/UnrealNetwork.h"

UHKAbilityTask_Attack::UHKAbilityTask_Attack()
{

}

UHKAbilityTask_Attack* UHKAbilityTask_Attack::CreateTask(UGameplayAbility* OwningAbility, TSubclassOf<class AHKTargetActorBase> TargetActorClass)
{
	UHKAbilityTask_Attack* NewTask = NewAbilityTask<UHKAbilityTask_Attack>(OwningAbility);
	NewTask->TargetActorClass = TargetActorClass;
	return NewTask;
}

void UHKAbilityTask_Attack::Activate()
{
	Super::Activate();
	
	PlayerCharacter = CastChecked<AHKPlayerCharacter>(GetAvatarActor());
	MuzzleLocation = PlayerCharacter->GetMesh()->GetSocketLocation("MuzzleFlash");
	MuzzleRotation = PlayerCharacter->GetControlRotation();

	SpawnedTargetActor = Cast<AHKTargetActorBase>(Ability->GetWorld()->SpawnActorDeferred<AGameplayAbilityTargetActor>(TargetActorClass, FTransform::Identity, nullptr, nullptr, ESpawnActorCollisionHandlingMethod::AlwaysSpawn));
	
	if (!PlayerCharacter->HasAuthority() && !SpawnedTargetActor->GetMadeLocal())
	{
		return;
	}

	if (SpawnedTargetActor)
	{
		SpawnedTargetActor->TargetDataReadyDelegate.AddUObject(this, &UHKAbilityTask_Attack::OnTargetDataReadyCallback);
	}

	UAbilitySystemComponent* ASC = AbilitySystemComponent.Get();

	if (ASC)
	{
		FTransform SpawnTransform = ASC->GetAvatarActor()->GetTransform();
		SpawnTransform.SetLocation(MuzzleLocation);
		SpawnTransform.SetRotation(MuzzleRotation.Quaternion());
		SpawnedTargetActor->InitTargetActor(Ability);
		SpawnedTargetActor->FinishSpawning(SpawnTransform);

		ASC->SpawnedTargetActors.Push(SpawnedTargetActor);
		SpawnedTargetActor->StartTargeting(Ability);
	}

	SetWaitingOnAvatar();
}

void UHKAbilityTask_Attack::OnDestroy(bool AbilityEnded)
{
	if (SpawnedTargetActor)
	{
		SpawnedTargetActor->Destroy();
	}

	Super::OnDestroy(AbilityEnded);
}

void UHKAbilityTask_Attack::OnTargetDataReadyCallback(const FGameplayAbilityTargetDataHandle& DataHandle)
{
	if (ShouldBroadcastAbilityTaskDelegates())
	{
		OnComplete.Broadcast(DataHandle);
	}

	EndTask();
}
