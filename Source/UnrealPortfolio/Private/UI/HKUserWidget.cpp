// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HKUserWidget.h"
#include "AbilitySystemBlueprintLibrary.h"

void UHKUserWidget::SetAbilitySystemComponent(AActor* InOwner)
{
	if (IsValid(InOwner))
	{
		ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(InOwner);
	}
}

UAbilitySystemComponent* UHKUserWidget::GetAbilitySystemComponent() const
{
	return ASC;
}
