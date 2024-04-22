// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "HKGameState.generated.h"

UENUM(BlueprintType)
enum class EMatchState : uint8
{
	None,
	Waiting,
	Playing,
	Ending,
	End
};

/**
 * 
 */
UCLASS()
class UNREALPORTFOLIO_API AHKGameState : public AGameStateBase
{
	GENERATED_BODY()
	
public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
public:
	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, Category="AAGameState")
	int32 AlivePlayerControllerCount;

	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, Category = "AAGameState")
	EMatchState MatchState = EMatchState::Waiting;

};
