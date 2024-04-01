// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbilityTargetActor.h"
#include "HKTargetActorBase.generated.h"

/**
 * 
 */
UCLASS()
class UNREALPORTFOLIO_API AHKTargetActorBase : public AGameplayAbilityTargetActor
{
	GENERATED_BODY()
	
public:
	AHKTargetActorBase();
	virtual void InitTargetActor(UGameplayAbility* Ability);
	virtual void StartTargeting(UGameplayAbility* Ability) override;
	virtual void ConfirmTargetingAndContinue() override;

	FORCEINLINE bool GetMadeLocal() const { return bMadeLocal; }

protected:
	virtual FGameplayAbilityTargetDataHandle MakeTargetData() const;
	
protected:
	UPROPERTY(EditAnywhere, Category = "AAGAS")
	TObjectPtr<class AHKPlayerCharacter> PlayerCharacter;

	UPROPERTY(EditAnywhere, Category = "AAGAS")
	TObjectPtr<class UCameraComponent> CameraComponent;

	UPROPERTY(EditAnywhere, Category = "AAGAS")
	TObjectPtr<class AHKWeapon> Weapon;

	UPROPERTY(EditAnywhere, Category = "AAGAS")
	FVector MuzzleLocation;

	UPROPERTY(EditAnywhere, Category = "AAGAS")
	FRotator MuzzleRotation;

	UPROPERTY(EditAnywhere, Category = "AAGAS")
	FVector CameraLocation;

	UPROPERTY(EditAnywhere, Category = "AAGAS")
	bool bMadeLocal;

};
