// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HKHpBarUserWidget.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystem/AttributeSet/HKCharacterAttributeSet.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

void UHKHpBarUserWidget::SetAbilitySystemComponent(AActor* InOwner)
{
	Super::SetAbilitySystemComponent(InOwner);

	if (ASC)
	{
		ASC->GetGameplayAttributeValueChangeDelegate(UHKCharacterAttributeSet::GetHealthAttribute()).AddUObject(this, &UHKHpBarUserWidget::OnHealthChanged);
		ASC->GetGameplayAttributeValueChangeDelegate(UHKCharacterAttributeSet::GetMaxHealthAttribute()).AddUObject(this, &UHKHpBarUserWidget::OnMaxHealthChanged);
		const UHKCharacterAttributeSet* CurrentAttributeSet = ASC->GetSet<UHKCharacterAttributeSet>();
		ensure(CurrentAttributeSet);

		CurrentHealth = CurrentAttributeSet->GetHealth();
		CurrentMaxHealth = CurrentAttributeSet->GetMaxHealth();
		ensure(CurrentMaxHealth > 0.0f);
		UpdateHpBar();
	}
}

void UHKHpBarUserWidget::OnHealthChanged(const FOnAttributeChangeData& ChangeData)
{
	CurrentHealth = ChangeData.NewValue;
	UpdateHpBar();
}

void UHKHpBarUserWidget::OnMaxHealthChanged(const FOnAttributeChangeData& ChangeData)
{
	CurrentMaxHealth = ChangeData.NewValue;
	UpdateHpBar();
}

void UHKHpBarUserWidget::UpdateHpBar()
{
	if (PbHpBar)
	{
		PbHpBar->SetPercent(CurrentHealth / CurrentMaxHealth);
	}

	if (TxtHpStat)
	{
		TxtHpStat->SetText(FText::FromString(FString::Printf(TEXT("%.0f/%0.f"), CurrentHealth, CurrentMaxHealth)));
	}
}
