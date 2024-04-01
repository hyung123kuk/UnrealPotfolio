// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/HKAnimInstance.h"
#include "GameFramework/Character.h"
#include "Characters/HKPlayerCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"


UHKAnimInstance::UHKAnimInstance()
{
	bIsAttack = false;
}

void UHKAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	Owner = Cast<ACharacter>(GetOwningActor());
	if (Owner)
	{
		Movement = Owner->GetCharacterMovement();
	}
}

void UHKAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (Movement)
	{
		Velocity = Movement->Velocity;
		bIsFalling = Movement->IsFalling();
		bIsJumping = bIsFalling & (Velocity.Z > 100.f);
		const AHKPlayerCharacter* PlayerCharacter = Cast<AHKPlayerCharacter>(Owner);
		if (PlayerCharacter != nullptr)
		{
			bool isMove = Velocity != FVector().Zero();
			InputMoveValue = isMove? PlayerCharacter->GetMoveValue():FVector2D().Zero();
			
			UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetOwningActor());
			if (ASC)
			{
				bIsAttack = ASC->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag(FName("Character.State.IsAttack")));
			}
			AHKWeapon* Weapon = PlayerCharacter->GetWeapon();
			if (Weapon)
			{
				WeaponType = Weapon->GetWeaponType();
			}
			ControlRotation = PlayerCharacter->GetLookValue();
		}
	}
}
