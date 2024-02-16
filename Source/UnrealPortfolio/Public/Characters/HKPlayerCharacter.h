// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/HKCharacterBase.h"
#include "EnhancedInputSubsystems.h"
#include "HKPlayerCharacter.generated.h"

/**
 * 
 */
UCLASS()
class UNREALPORTFOLIO_API AHKPlayerCharacter : public AHKCharacterBase
{
	GENERATED_BODY()
	
public:
	AHKPlayerCharacter();

	FORCEINLINE const FVector2D& GetMoveValue() const { return InputMoveValue; }

	FORCEINLINE const FRotator& GetLookValue() const { return InputLookValue; }

protected:
	virtual void BeginPlay() override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

private:
	void Move(const FInputActionValue& Value);

	void Look(const FInputActionValue& InValue);

private:
	UFUNCTION(Server, Unreliable)
	void UpdateInputMoveValue_Server(const FVector2D& OwnerInputMoveValue);

	UFUNCTION(Server, Unreliable)
	void UpdateInputLookValue_Server(const FRotator& OwnerInputLookValue);

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AACamera", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USpringArmComponent> SpringArmComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AACamera", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UCameraComponent> CameraComponent;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AAInput", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputMappingContext> DefaultMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AAInput", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AAInput", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> LookAction;

private:
	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, Category = "AAInput", meta = (AllowPrivateAccess = true))
	FVector2D InputMoveValue;

	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, Category = "AAInput", meta = (AllowPrivateAccess = true))
	FRotator InputLookValue;
};
