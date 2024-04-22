// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HKGameResultWidget.generated.h"

/**
 * 
 */
UCLASS()
class UNREALPORTFOLIO_API UHKGameResultWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;
	
private:
	UFUNCTION()
	void OnReturnToLobbyButtonClicked();
	
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AAWidget", Meta = (BindWidget))
	TObjectPtr<class UTextBlock> RankingText;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AAWidget", Meta = (BindWidget))
	TObjectPtr<class UButton> ReturnToLobbyButton;

};
