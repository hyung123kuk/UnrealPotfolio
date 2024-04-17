// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HKLobbyLevelUI.generated.h"



/**
 * 
 */
UCLASS()
class UNREALPORTFOLIO_API UHKLobbyLevelUI : public UUserWidget
{
	GENERATED_BODY()
	
protected:
    virtual void NativeConstruct() override;
	
private:
    UFUNCTION()
    void OnRedTeamButtonClicked();

    UFUNCTION()
    void OnBlueTeamButtonClicked();

    UFUNCTION()
    void OnSubmitButtonClicked();
	
protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AALobbyUI", Meta = (AllowPrivateAccess, BindWidget))
    TObjectPtr<class UButton> RedTeamButton;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AALobbyUI", Meta = (AllowPrivateAccess, BindWidget))
    TObjectPtr<class UButton> BlueTeamButton;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AALobbyUI", Meta = (AllowPrivateAccess, BindWidget))
    TObjectPtr<class UButton> SubmitButton;

    int IsTeam;

};
