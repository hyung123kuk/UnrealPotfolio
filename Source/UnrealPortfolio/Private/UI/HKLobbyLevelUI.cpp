// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HKLobbyLevelUI.h"
#include "Components/Button.h"
#include "EngineUtils.h"
#include "Components/EditableText.h"
#include "Kismet/GameplayStatics.h"
#include "Controllers/HKUIPlayerController.h"
#include "UnrealPortfolio/UnrealPortfolio.h"

void UHKLobbyLevelUI::NativeConstruct()
{
    Super::NativeConstruct();
    SubmitButton.Get()->OnClicked.AddDynamic(this, &ThisClass::OnSubmitButtonClicked);
}

void UHKLobbyLevelUI::OnSubmitButtonClicked()
{
   //UGameplayStatics::OpenLevel(GetWorld(), TEXT("Loading"), true, FString(TEXT("NextLevel=Example?Saved=false")));
    AHKUIPlayerController* PlayerController = GetOwningPlayer<AHKUIPlayerController>();
    if (IsValid(PlayerController) == true)
    {
        FText ServerIP = EditServerIP->GetText();
        PlayerController->JoinServer(ServerIP.ToString());
    }
}
