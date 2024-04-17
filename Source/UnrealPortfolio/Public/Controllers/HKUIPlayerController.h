// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "HKUIPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class UNREALPORTFOLIO_API AHKUIPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	virtual void BeginPlay() override;

	void JoinServer(const FString& InIPAddress);

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AAUIController", Meta = (AllowPrivateAccess))
	TSubclassOf<class UUserWidget> UIWidgetClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AAUIController", Meta = (AllowPrivateAccess))
	TObjectPtr<class UUserWidget> UIWidgetInstance;
	
};
