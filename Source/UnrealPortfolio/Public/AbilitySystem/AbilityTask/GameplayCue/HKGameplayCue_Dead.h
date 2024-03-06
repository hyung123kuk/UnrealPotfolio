// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayCueNotify_Static.h"
#include "HKGameplayCue_Dead.generated.h"

/**
 * 
 */
UCLASS()
class UHKGameplayCue_Dead : public UGameplayCueNotify_Static
{
	GENERATED_BODY()
	
public:
	UHKGameplayCue_Dead();
	
	virtual bool OnExecute_Implementation(AActor* Target, const FGameplayCueParameters& Parameters) const override;
};
