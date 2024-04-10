// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/HKUserWidget.h"
#include "GameplayEffectTypes.h"
#include "HKHpBarUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class UNREALPORTFOLIO_API UHKHpBarUserWidget : public UHKUserWidget
{
	GENERATED_BODY()
	
	
public:
	virtual void SetAbilitySystemComponent(AActor* InOwner) override;

protected:
	virtual void OnHealthChanged(const FOnAttributeChangeData& ChangeData);
	virtual void OnMaxHealthChanged(const FOnAttributeChangeData& ChangeData);

	void UpdateHpBar();

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UProgressBar> PbHpBar;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> TxtHpStat;
	float CurrentHealth = 0.0f;
	float CurrentMaxHealth = 0.1f;

};
