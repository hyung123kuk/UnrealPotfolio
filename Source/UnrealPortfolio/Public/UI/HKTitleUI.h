// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HKTitleUI.generated.h"

/**
 * 
 */
UCLASS()
class UNREALPORTFOLIO_API UHKTitleUI : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UHKTitleUI(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	
protected:
	virtual void NativeConstruct() override;
	
private:
	UFUNCTION()
	void OnNewGameButtonClicked();

	UFUNCTION()
	void OnExitGameButtonClicked();

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AATitleUI", Meta = (AllowPrivateAccess, BindWidget))
	TObjectPtr<class UButton> NewGameButton;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AATitleUI", Meta = (AllowPrivateAccess, BindWidget))
	TObjectPtr<class UButton> ExitGameButton;

};
