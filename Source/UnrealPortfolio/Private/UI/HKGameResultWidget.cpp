// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HKGameResultWidget.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"

void UHKGameResultWidget::NativeConstruct()
{
    Super::NativeConstruct();

    if (false == ReturnToLobbyButton.Get()->OnClicked.IsAlreadyBound(this, &ThisClass::OnReturnToLobbyButtonClicked))
    {
        ReturnToLobbyButton.Get()->OnClicked.AddDynamic(this, &ThisClass::OnReturnToLobbyButtonClicked);
    }
}

void UHKGameResultWidget::OnReturnToLobbyButtonClicked()
{
    UGameplayStatics::OpenLevel(GetWorld(), FName(TEXT("Loading")), true, FString(TEXT("NextLevel=Lobby?Saved=false")));
}