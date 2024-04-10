// Fill out your copyright notice in the Description page of Project Settings.


#include "Controllers/HKPlayerControllerBase.h"
#include "Characters/HKPlayerCharacter.h"
#include "Camera/CameraShakeBase.h"
#include "UI/HKHUD.h"
#include "Game/HKPlayerState.h"


void AHKPlayerControllerBase::BeginPlay()
{
	Super::BeginPlay();

	//bShowMouseCursor = true;
	//DefaultMouseCursor = EMouseCursor::None;

	FInputModeGameAndUI InputModeData;
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	InputModeData.SetHideCursorDuringCapture(false);
	//SetInputMode(InputModeData);
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
}

