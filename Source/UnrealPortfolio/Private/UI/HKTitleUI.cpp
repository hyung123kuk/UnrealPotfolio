// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HKTitleUI.h"

#include "Components/Button.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"

UHKTitleUI::UHKTitleUI(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UHKTitleUI::NativeConstruct()
{
	NewGameButton.Get()->OnClicked.AddDynamic(this, &ThisClass::OnNewGameButtonClicked);
	ExitGameButton.Get()->OnClicked.AddDynamic(this, &ThisClass::OnExitGameButtonClicked);
	SingleGameButton.Get()->OnClicked.AddDynamic(this, &ThisClass::OnSingleGameButtonClicked);
}

void UHKTitleUI::OnNewGameButtonClicked()
{
	UGameplayStatics::OpenLevel(GetWorld(),TEXT("Loading"), true, FString(TEXT("NextLevel=Lobby?Saved=false")));
}

void UHKTitleUI::OnExitGameButtonClicked()
{
	UKismetSystemLibrary::QuitGame(this, GetOwningPlayer(), EQuitPreference::Quit, false);
}

void UHKTitleUI::OnSingleGameButtonClicked()
{
	UGameplayStatics::OpenLevel(GetWorld(), TEXT("Loading"), true, FString(TEXT("NextLevel=Main?Saved=false")));
}
