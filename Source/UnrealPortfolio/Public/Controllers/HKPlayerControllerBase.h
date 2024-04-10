// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "HKPlayerControllerBase.generated.h"

class UHKHUD;
class UCameraShakeBase;
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
	void ShowHUD();
	UHKHUD* GetHUDWidget() const { return HUDWidget; }

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AACamera", Meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UCameraShakeBase> FireShake;

	UPROPERTY();
	TObjectPtr<UHKHUD> HUDWidget;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "ASPlayerController", Meta = (AllowPrivateAccess));
	TSubclassOf<UHKHUD> HUDWidgetClass;
};
