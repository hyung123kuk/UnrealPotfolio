// Fill out your copyright notice in the Description page of Project Settings.


#include "Controllers/HKPlayerControllerBase.h"
#include "Camera/CameraShakeBase.h"

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

