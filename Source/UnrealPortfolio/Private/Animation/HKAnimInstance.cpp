// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/HKAnimInstance.h"
#include "GameFramework/Character.h"
#include "Characters/HKPlayerCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

UHKAnimInstance::UHKAnimInstance()
{
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

			ControlRotation = PlayerCharacter->GetLookValue();
		}
	}
}
