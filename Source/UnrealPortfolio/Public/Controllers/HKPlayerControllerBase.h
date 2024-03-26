// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "HKPlayerControllerBase.generated.h"

/**
 * 
 */
UCLASS()
class UNREALPORTFOLIO_API AHKPlayerControllerBase : public APlayerController
{
	GENERATED_BODY()
	
protected:
	void BeginPlay() override;
	
public:
	void CameraShake(float Strength);

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AACamera", Meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class UCameraShakeBase> FireShake;

};
