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
	ACharacter* Character = Cast<ACharacter>(Target);
	if (Character)
	{
		Character->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		Character->GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		Character->GetMesh()->SetSimulatePhysics(true);
	}


	return true;
}
