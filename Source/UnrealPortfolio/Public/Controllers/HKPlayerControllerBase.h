// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "HKPlayerControllerBase.generated.h"

class UHKHUD;
class UCameraShakeBase;
class UHKGameResultWidget;

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

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION(Client, Reliable)
	void ShowWinnerUI();

	UFUNCTION(Client, Reliable)
	void ShowLooserUI(int32 InRanking);

	UFUNCTION(Client, Reliable)
	void ReturnToLobby();

public:
	UPROPERTY(Replicated, EditDefaultsOnly, BlueprintReadWrite, Category = "AAWidget", Meta = (AllowPrivateAccess))
	FText NotificationText;

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AACamera", Meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UCameraShakeBase> FireShake;

	UPROPERTY();
	TObjectPtr<UHKHUD> HUDWidget;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AAWidget", Meta = (AllowPrivateAccess))
	TSubclassOf<UHKHUD> HUDWidgetClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AAWidget", Meta = (AllowPrivateAccess))
	TSubclassOf<UUserWidget> NotificationTextUIClass;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AAWidget", Meta = (AllowPrivateAccess))
	TSubclassOf<UHKGameResultWidget> WinnerUIClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AAWidget", Meta = (AllowPrivateAccess))
	TSubclassOf<UHKGameResultWidget> LooserUIClass;

};
