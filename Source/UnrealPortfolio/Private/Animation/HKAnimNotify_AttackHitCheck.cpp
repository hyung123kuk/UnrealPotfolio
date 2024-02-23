// Fill out your copyright notice in the Description page of Project Settings.

#include "Animation/HKAnimNotify_AttackHitCheck.h"
#include "AbilitySystemBlueprintLibrary.h"

UHKAnimNotify_AttackHitCheck::UHKAnimNotify_AttackHitCheck()
{
}

FString UHKAnimNotify_AttackHitCheck::GetNotifyName_Implementation() const
{
	return TEXT("AttackHitCheck");
}

void UHKAnimNotify_AttackHitCheck::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (MeshComp)
	{
		AActor* OwnerActor = MeshComp->GetOwner();
		if (OwnerActor)
		{
			FGameplayEventData PayloadData;
			UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(OwnerActor, TriggerGameplayTag, PayloadData);
		}
	}
}
