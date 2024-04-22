// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/HKGameState.h"
#include "Net/UnrealNetwork.h"
#include "Engine/Engine.h"

void AHKGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, AlivePlayerControllerCount);
	DOREPLIFETIME(ThisClass, MatchState);
}
