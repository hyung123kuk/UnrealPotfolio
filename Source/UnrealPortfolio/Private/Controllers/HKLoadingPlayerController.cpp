// Fill out your copyright notice in the Description page of Project Settings.


#include "Controllers/HKLoadingPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/GameModeBase.h"

void AHKLoadingPlayerController::BeginPlay()
{
	Super::BeginPlay();
    AGameModeBase* GM = UGameplayStatics::GetGameMode(this);
    if (true == ::IsValid(GM))
    {
        FString NextLevelString = UGameplayStatics::ParseOption(GM->OptionsString, FString(TEXT("NextLevel")));
        UGameplayStatics::OpenLevel(GM, *NextLevelString, false);
    }
}
