// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "HKGameModeBase.generated.h"

class UAbilitySystemComponent;


/**
 * 
 */
UCLASS()
class UNREALPORTFOLIO_API AHKGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	AHKGameModeBase();

	virtual void PostLogin(APlayerController* NewPlayer) override;
	virtual void Logout(AController* Exiting) override;
	virtual void BeginPlay() override;

private:
	UFUNCTION()
	void OnMainTimerElapsed();

	void NotifyToAllPlayer(const FString& NotificationString);

	UFUNCTION()
	void OnControllerDead(UAbilitySystemComponent* DeadAbilitySystemComponent);

public:
	FTimerHandle MainTimerHandle;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AAGameMode")
	int32 WaitingTime = 15;

	int32 RemainWaitingTimeForPlaying = 15;

	int32 MinimumPlayerCountForPlaying = 2;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AAGameMode")
	int32 EndingTime = 15;

	int32 RemainWaitingTimeForEnding = 15;
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AAGameMode", Meta = (AllowPrivateAccess))
	TArray<TObjectPtr<class AHKPlayerControllerBase>> AlivePlayerControllers;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AAGameMode", Meta = (AllowPrivateAccess))
	TArray<TObjectPtr<class AHKPlayerControllerBase>> DeadPlayerControllers;


};
