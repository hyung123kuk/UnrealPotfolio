// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Item/HKWeapon.h"
#include "HKAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class UNREALPORTFOLIO_API UHKAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	UHKAnimInstance();

protected:
	virtual void NativeInitializeAnimation() override;

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AAAnimation")
	TObjectPtr<class ACharacter> Owner;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AAAnimation")
	TObjectPtr<class UCharacterMovementComponent> Movement;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AAAnimation")
	uint8 bIsFalling : 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AAAnimation")
	uint8 bIsJumping : 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AAAnimation")
	FVector Velocity;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AAAnimation")
	FRotator ControlRotation;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AAAnimation")
	FVector2D InputMoveValue;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AAAnimation")
	uint8 bIsAttack : 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AAAnimation")
	EWeaponType WeaponType = EWeaponType::MeleeWeapon;

};
