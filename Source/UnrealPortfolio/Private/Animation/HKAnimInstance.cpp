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
		ControlRotation = Owner->GetControlRotation();

		const FVector CharacterMovementVector = Owner->GetLastMovementInputVector();
		bool isMove = (CharacterMovementVector != FVector().Zero());
		if (isMove)
		{
			const AHKPlayerCharacter* PlayerCharacter = Cast<AHKPlayerCharacter>(Owner);
			if (PlayerCharacter != nullptr)
			{
				InputMoveValue = PlayerCharacter->GetInputValue();
			}
		}
		else
		{
			InputMoveValue = FVector2D().Zero();
		}
	}
}
