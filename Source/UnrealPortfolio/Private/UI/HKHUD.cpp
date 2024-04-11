// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HKHUD.h"
#include "UI/HKHpBarUserWidget.h"
#include "UI/HKInventoryWidget.h"

void UHKHUD::SetHUD(AActor* InOwner)
{
	HpBar->SetAbilitySystemComponent(InOwner);
	Inventory->SetAbilitySystemComponent(InOwner);
}
