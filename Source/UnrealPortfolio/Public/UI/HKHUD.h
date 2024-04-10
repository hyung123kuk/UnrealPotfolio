// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HKHUD.generated.h"


class UHKHpBarUserWidget;
/**
 * 
 */
UCLASS()
class UNREALPORTFOLIO_API UHKHUD : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void SetHUD(AActor* InOwner);
	
protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UHKHpBarUserWidget> HpBar;

};
