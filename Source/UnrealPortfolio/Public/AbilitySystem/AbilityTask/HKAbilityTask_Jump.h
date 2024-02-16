// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "HKAbilityTask_Jump.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FLandafterjumpingDelegate);


/**
 * 
 */
UCLASS()
class UNREALPORTFOLIO_API UHKAbilityTask_Jump : public UAbilityTask
{
	GENERATED_BODY()
	
public:
	UHKAbilityTask_Jump();

	UFUNCTION(BlueprintCallable, Category = "Ability|Tasks", meta = (DisplayName = "JumpAndWaitForLanding", HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "TRUE"))
	static UHKAbilityTask_Jump* CreateTast(UGameplayAbility* OwningAbility);
	
	virtual void Activate() override;
	virtual void OnDestroy(bool AbilityEnded) override;

	UPROPERTY(BlueprintAssignable)
	FLandafterjumpingDelegate OnComplete;

protected:
	UFUNCTION()
	void OnLandedCallback(const FHitResult& Hit);

};
