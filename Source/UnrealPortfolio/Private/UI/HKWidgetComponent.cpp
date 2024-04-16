// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HKWidgetComponent.h"
#include "UI/HKUserWidget.h"

void UHKWidgetComponent::InitWidget()
{
	Super::InitWidget();

	UHKUserWidget* UserWidget = Cast<UHKUserWidget>(GetWidget());
	if (UserWidget)
	{
		UserWidget->SetAbilitySystemComponent(GetOwner());
	}
}
