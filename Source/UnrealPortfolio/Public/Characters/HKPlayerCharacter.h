// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/HKCharacterBase.h"
#include "EnhancedInputSubsystems.h"
#include "AbilitySystemInterface.h"
#include "HKPlayerCharacter.generated.h"

/**
 * 
 */
UCLASS()
class UNREALPORTFOLIO_API AHKPlayerCharacter : public AHKCharacterBase, public IAbilitySystemInterface
{
	GENERATED_BODY()
	
public:
	AHKPlayerCharacter();

	FORCEINLINE const FVector2D& GetMoveValue() const { return InputMoveValue; }

	FORCEINLINE const FRotator& GetLookValue() const { return InputLookValue; }

protected:
	virtual void BeginPlay() override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;

//Ability Func
protected:	
	virtual class UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	void GASInputPressed(int32 InputId);
	void GASInputReleased(int32 InputId);

//Behaviour Func
private:
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& InValue);

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

//Input Action
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AAInput", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AAInput", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> LookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AAInput", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> JumpAction;

//Ability Params
protected:
	UPROPERTY(EditAnywhere, Category = "AGAS")
	TObjectPtr<class UAbilitySystemComponent> ASC;

	UPROPERTY(EditAnywhere, Category = "AGAS")
	TMap<int32, TSubclassOf<class UGameplayAbility>> StartInputAbilities;


//Behaviour Params
private:
	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, Category = "AAInput", meta = (AllowPrivateAccess = true))
	FVector2D InputMoveValue;

	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, Category = "AAInput", meta = (AllowPrivateAccess = true))
	FRotator InputLookValue;

};
