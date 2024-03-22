// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/HKCharacterBase.h"
#include "EnhancedInputSubsystems.h"
#include "AbilitySystemInterface.h"
#include "HKPlayerCharacter.generated.h"

class AHKWeapon;

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
	AHKWeapon* GetWeapon() const { return EquipWeapon; }
	

protected:
	virtual void BeginPlay() override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;

//Ability Func
protected:	
	void InitAbilityActorInfo();
	virtual class UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	void GASInputPressed(int32 InputId);
	void GASInputReleased(int32 InputId);

//Behaviour Func
private:
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void Zoom(const FInputActionValue& Value);

	UFUNCTION()
	void ChangeWeapon(const FInputActionValue& Value);

	UFUNCTION(Server, Unreliable)
	void UpdateInputMoveValue_Server(const FVector2D& OwnerInputMoveValue);
	UFUNCTION(Server, Unreliable)
	void UpdateInputLookValue_Server(const FRotator& OwnerInputLookValue);

	
	UFUNCTION(Server, Reliable)
	void UpdateChangeWeapon_Server(const int32& InputKey);
	UFUNCTION()
	void OnRep_WeaponNum();

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AACamera", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USpringArmComponent> SpringArmComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AACamera", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UCameraComponent> CameraComponent;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AAInput", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputMappingContext> DefaultMappingContext;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadOnly, Category = "AAWeapon", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USkeletalMeshComponent> WeaponSocket_R;

	UPROPERTY(EditAnywhere, Category = "AAWeapon")
	TObjectPtr<class AHKWeapon> EquipWeapon;

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

//Ability Params
protected:
	UPROPERTY(EditAnywhere, Category = "AAGAS")
	TObjectPtr<class UAbilitySystemComponent> ASC;

	UPROPERTY(EditAnywhere, Category = "AAGAS")
	TObjectPtr<class UHKCharacterAttributeSet> CharacterAttributeSet;

	UPROPERTY(EditAnywhere, Category = "AAGAS")
	TMap<int32, TSubclassOf<class UGameplayAbility>> StartInputAbilities;

	UPROPERTY(EditAnywhere, Category = "AAGAS")
	TArray<TSubclassOf<class UGameplayAbility>> StartAbilities;

	UPROPERTY(EditAnywhere, Category = "AAGAS")
	TMap<int32, TSubclassOf<class AHKWeapon>> SwapWeapons;


//Behaviour Params
private:
	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, Category = "AAInput", Meta = (AllowPrivateAccess = "true"))
	FVector2D InputMoveValue;

	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, Category = "AAInput", Meta = (AllowPrivateAccess = "true"))
	FRotator InputLookValue;

	UPROPERTY(ReplicatedUsing = OnRep_WeaponNum, VisibleAnywhere, BlueprintReadOnly, Category = "AAInput", Meta = (AllowPrivateAccess = "true"))
	int32 WeaponNum;

	friend class AHKTargetActor_Shot;
};
