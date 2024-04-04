// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/HKCharacterBase.h"
#include "EnhancedInputSubsystems.h"
#include "AbilitySystemInterface.h"
#include "HKPlayerCharacter.generated.h"

class AHKWeapon;
class UCameraComponent;
class UAbilitySystemComponent;
class AHKWeapon;
class USpringArmComponent;

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

	FORCEINLINE const bool GetIsZoom() const { return IsZoom; }
	FORCEINLINE void SetIsZoom(bool NewIsZoom) { IsZoom = NewIsZoom; }

	TMap<int32, TSubclassOf<AHKWeapon>> GetSwapWeapons() { return SwapWeapons; }
	AHKWeapon* GetWeapon() const { return EquipWeapon; }
	void SetWeapon(AHKWeapon* EquipWeapon);

	UCameraComponent* GetCameraComponent() const { return CameraComponent; }
	USpringArmComponent* GetSpringArmComponent() const { return SpringArmComponent; }
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

protected:
	virtual void BeginPlay() override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;

//Ability Func
protected:	
	void InitAbilityActorInfo();

	void GASInputPressed(int32 InputId);
	void GASInputReleased(int32 InputId);

//Behaviour Func
private:
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void ChangeWeapon(const FInputActionValue& Value);

	UFUNCTION()
	void OnRep_ChangeWeapon();

	UFUNCTION(Server, Unreliable)
	void UpdateInputMoveValue_Server(const FVector2D& OwnerInputMoveValue);
	UFUNCTION(Server, Unreliable)
	void UpdateInputLookValue_Server(const FRotator& OwnerInputLookValue);


private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AACamera", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USpringArmComponent> SpringArmComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AACamera", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCameraComponent> CameraComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AAInput", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputMappingContext> DefaultMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AAWeapon", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USkeletalMeshComponent> WeaponSocket_R;

	UPROPERTY(ReplicatedUsing = OnRep_ChangeWeapon, EditAnywhere, Category = "AAWeapon")
	TObjectPtr<AHKWeapon> EquipWeapon;

//Input Action
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AAInput", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AAInput", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> LookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AAInput", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> JumpAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AAInput", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> AttackAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AAInput", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> ChangeWeaponAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AAInput", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> ZoomAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AAInput", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> ReloadAction;

//Ability Params
protected:
	UPROPERTY(EditAnywhere, Category = "AAGAS")
	TObjectPtr<UAbilitySystemComponent> ASC;

	UPROPERTY(EditAnywhere, Category = "AAGAS")
	TObjectPtr<class UHKCharacterAttributeSet> CharacterAttributeSet;

	UPROPERTY(EditAnywhere, Category = "AAGAS")
	TMap<int32, TSubclassOf<class UGameplayAbility>> StartInputAbilities;

	UPROPERTY(EditAnywhere, Category = "AAGAS")
	TArray<TSubclassOf<class UGameplayAbility>> StartAbilities;

	UPROPERTY(EditAnywhere, Category = "AAGAS")
	TMap<int32, TSubclassOf<AHKWeapon>> SwapWeapons;


//Behaviour Params
private:
	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, Category = "AAInput", Meta = (AllowPrivateAccess = "true"))
	FVector2D InputMoveValue;

	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, Category = "AAInput", Meta = (AllowPrivateAccess = "true"))
	FRotator InputLookValue;

	bool IsZoom;


	friend class AHKTargetActor_Shot;
};
