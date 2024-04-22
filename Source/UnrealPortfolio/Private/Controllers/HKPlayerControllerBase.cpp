// Fill out your copyright notice in the Description page of Project Settings.


#include "Controllers/HKPlayerControllerBase.h"
#include "Characters/HKPlayerCharacter.h"
#include "Camera/CameraShakeBase.h"
#include "UI/HKHUD.h"
#include "Game/HKPlayerState.h"
#include "Net/UnrealNetwork.h"
#include "Engine/Engine.h"
#include "UI/HKGameResultWidget.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"

void AHKPlayerControllerBase::BeginPlay()
{
	Super::BeginPlay();

    if (HasAuthority())
        return;

	//bShowMouseCursor = true;
	//DefaultMouseCursor = EMouseCursor::None;

	FInputModeGameAndUI InputModeData;
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	InputModeData.SetHideCursorDuringCapture(false);
	//SetInputMode(InputModeData);
}

void AHKPlayerControllerBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(ThisClass, NotificationText);
}

void AHKPlayerControllerBase::ShowWinnerUI_Implementation()
{
    if (false == HasAuthority())
    {
        if (true == ::IsValid(WinnerUIClass))
        {
            UHKGameResultWidget* WinnerUI = CreateWidget<UHKGameResultWidget>(this, WinnerUIClass);
            if (true == ::IsValid(WinnerUI))
            {
                WinnerUI->AddToViewport(3);
                WinnerUI->RankingText->SetText(FText::FromString(TEXT("#01")));

                /*FInputModeUIOnly Mode;
                Mode.SetWidgetToFocus(WinnerUI->GetCachedWidget());
                SetInputMode(Mode);*/

                bShowMouseCursor = true;
            }
        }
    }
}

void AHKPlayerControllerBase::ShowLooserUI_Implementation(int32 InRanking)
{
    if (false == HasAuthority())
    {
        if (true == ::IsValid(LooserUIClass))
        {
            UHKGameResultWidget* LooserUI = CreateWidget<UHKGameResultWidget>(this, LooserUIClass);
            if (true == ::IsValid(LooserUI))
            {
                LooserUI->AddToViewport(3);
                FString RankingString = FString::Printf(TEXT("#%02d"), InRanking);
                LooserUI->RankingText->SetText(FText::FromString(RankingString));

                /*FInputModeUIOnly Mode;
                Mode.SetWidgetToFocus(LooserUI->GetCachedWidget());
                SetInputMode(Mode);*/

                bShowMouseCursor = true;
            }
        }
    }
}

void AHKPlayerControllerBase::ReturnToLobby_Implementation()
{
    if (false == HasAuthority())
    { // 서버의 레벨이 변경되는걸 원치 않음. 클라가 이동해야하므로 if() 처리 필수.
        UGameplayStatics::OpenLevel(GetWorld(), FName(TEXT("Loading")), true, FString(TEXT("NextLevel=Lobby?Saved=false")));
    }
}

void AHKPlayerControllerBase::CameraShake(float Strength)
{
	ClientPlayCameraShake(FireShake, Strength);
}

void AHKPlayerControllerBase::ShowHUD()
{
    if (true == ::IsValid(HUDWidgetClass))
    {
        HUDWidget = CreateWidget<UHKHUD>(this, HUDWidgetClass);
        if (true == ::IsValid(HUDWidget))
        {
            HUDWidget->AddToViewport();
            AHKPlayerCharacter* PC = GetPawn<AHKPlayerCharacter>();
            HUDWidget->SetHUD(PC);
        }
    }

    if (true == ::IsValid(NotificationTextUIClass))
    {
        UUserWidget* NotificationTextUI = CreateWidget<UUserWidget>(this, NotificationTextUIClass);
        if (true == ::IsValid(NotificationTextUI))
        {
            NotificationTextUI->AddToViewport(1);
            NotificationTextUI->SetVisibility(ESlateVisibility::Visible);
        }
    }
}



