// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "HKCharacterBase.generated.h"

UCLASS()
class UNREALPORTFOLIO_API AHKCharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AHKCharacterBase();

public:
	FORCEINLINE virtual class UAnimMontage* GetShotMontage() const { return ShotMontage; }
	FORCEINLINE virtual class UAnimMontage* GetReloadMontage() const { return ReloadMontage; }
	FORCEINLINE virtual class UAnimMontage* GetDeadMontage() const { return DeadMontage; }
	FORCEINLINE virtual class UAnimMontage* GetSwapMontage() const { return SwapMontage; }

	FORCEINLINE virtual void SetShotMontage(UAnimMontage* NewShotMontage) { ShotMontage = NewShotMontage; }
	FORCEINLINE virtual void SetReloadMontage(UAnimMontage* NewReloadMontage) { ReloadMontage = NewReloadMontage; }
	FORCEINLINE virtual void SetSwapMontage(UAnimMontage* NewSwapMontage) { SwapMontage = NewSwapMontage; }
	
protected:


protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AAAnimation")
	TObjectPtr<class UAnimMontage> ShotMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AAAnimation")
	TObjectPtr<class UAnimMontage> ReloadMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AAAnimation")
	TObjectPtr<class UAnimMontage> DeadMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AAAnimation")
	TObjectPtr<class UAnimMontage> SwapMontage;
};
