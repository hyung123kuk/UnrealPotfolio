// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Controllers/HKUIPlayerController.h"
#include "HKLoadingPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class UNREALPORTFOLIO_API AHKLoadingPlayerController : public AHKUIPlayerController
{
	GENERATED_BODY()
	
public:
	virtual void BeginPlay() override;
	
};
