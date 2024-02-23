// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "GamePlayTagContainer.h"
#include "HKAnimNotify_AttackHitCheck.generated.h"

/**
 * 
 */
UCLASS()
class UNREALPORTFOLIO_API UHKAnimNotify_AttackHitCheck : public UAnimNotify
{
	GENERATED_BODY()
	
public:
	UHKAnimNotify_AttackHitCheck();

protected:
	virtual FString GetNotifyName_Implementation() const override;
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

protected:

	UPROPERTY(EditAnywhere, Meta = (Categories = Event))
	FGameplayTag TriggerGameplayTag;
	
};
