// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "HKAbilityTask_Shot.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FShotResultDelegate, const FGameplayAbilityTargetDataHandle&, TargetDataHandle);


/**
 * 
 */
UCLASS()
class UNREALPORTFOLIO_API UHKAbilityTask_Shot : public UAbilityTask
{
	GENERATED_BODY()
	
public:
	UHKAbilityTask_Shot();

	UFUNCTION(BlueprintCallable, Category = "AATask", meta = (DisplayName = "ShotTask", HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "TRUE"))
	static UHKAbilityTask_Shot* CreateTask(UGameplayAbility* OwningAbility, TSubclassOf<class AHKTargetActor_Shot> TargetActorClass);
	
	virtual void Activate() override;
	virtual void OnDestroy(bool AbilityEnded) override;

protected:
	void OnTargetDataReadyCallback(const FGameplayAbilityTargetDataHandle& DataHandle);

public:
	UPROPERTY(BlueprintAssignable)
	FShotResultDelegate OnComplete;

protected:
	UPROPERTY()
	TSubclassOf<class AHKTargetActor_Shot> TargetActorClass;

	UPROPERTY()
	TObjectPtr<class AHKTargetActor_Shot> SpawnedTargetActor;
};
