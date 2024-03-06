// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/AbilityTask/GameplayCue/HKGameplayCue_Dead.h"
#include "Characters/HKCharacterBase.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

UHKGameplayCue_Dead::UHKGameplayCue_Dead()
{
}

bool UHKGameplayCue_Dead::OnExecute_Implementation(AActor* Target, const FGameplayCueParameters& Parameters) const
{
	// TODO : Dead Effect
	ACharacter* Characrter = Cast<ACharacter>(Target);
	if (Characrter)
	{
		Characrter->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		Characrter->GetMesh()->SetSimulatePhysics(true);
	}


	return true;
}
