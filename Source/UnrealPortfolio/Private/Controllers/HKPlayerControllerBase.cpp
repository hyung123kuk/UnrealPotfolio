// Fill out your copyright notice in the Description page of Project Settings.


#include "Controllers/HKPlayerControllerBase.h"

void AHKPlayerControllerBase::BeginPlay()
{
	Super::BeginPlay();

	FInputModeGameOnly GameOnlyInputMode;
	SetInputMode(GameOnlyInputMode);
}
