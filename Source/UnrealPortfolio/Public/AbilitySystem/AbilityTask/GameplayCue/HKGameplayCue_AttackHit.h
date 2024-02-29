// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayCueNotify_Static.h"
#include "Sound/SoundCue.h"
#include "HKGameplayCue_AttackHit.generated.h"

/**
 * 
 */
UCLASS()
class UNREALPORTFOLIO_API UHKGameplayCue_AttackHit : public UGameplayCueNotify_Static
{
	GENERATED_BODY()
	
public:
	UHKGameplayCue_AttackHit();

	virtual bool OnExecute_Implementation(AActor* Target, const FGameplayCueParameters& Parameters) const override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AAGameplayCue")
	TObjectPtr<class UParticleSystem> ParticleSystem;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AAGameplayCue")
	TObjectPtr<class USoundWave> HitSound;
};
