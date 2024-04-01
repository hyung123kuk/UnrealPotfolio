// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "HKAbilityTask_Attack.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FShotResultDelegate, const FGameplayAbilityTargetDataHandle&, TargetDataHandle);


/**
 * 
 */
UCLASS()
class UNREALPORTFOLIO_API UHKAbilityTask_Attack : public UAbilityTask
{
	GENERATED_BODY()
	
public:
	UHKAbilityTask_Attack();

	UFUNCTION(BlueprintCallable, Category = "AATask", meta = (DisplayName = "ShotTask", HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "TRUE"))
	static UHKAbilityTask_Attack* CreateTask(UGameplayAbility* OwningAbility, TSubclassOf<class AHKTargetActorBase> TargetActorClass);
	
	virtual void Activate() override;
	virtual void OnDestroy(bool AbilityEnded) override;

protected:
	void OnTargetDataReadyCallback(const FGameplayAbilityTargetDataHandle& DataHandle);

public:
	UPROPERTY(BlueprintAssignable)
	FShotResultDelegate OnComplete;

protected:
	UPROPERTY()
	TSubclassOf<class AHKTargetActorBase> TargetActorClass;

	UPROPERTY()
	TObjectPtr<class AHKTargetActorBase> SpawnedTargetActor;

	UPROPERTY(EditAnywhere, Category = "AAGAS")
	TObjectPtr<class AHKPlayerCharacter> PlayerCharacter;

	UPROPERTY(EditAnywhere, Category = "AAGAS")
	FVector MuzzleLocation;

	UPROPERTY(EditAnywhere, Category = "AAGAS")
	FRotator MuzzleRotation;
};
