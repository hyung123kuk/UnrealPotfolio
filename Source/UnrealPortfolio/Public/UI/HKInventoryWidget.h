// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/HKUserWidget.h"
#include "HKInventoryWidget.generated.h"

class UImage;
class AHKWeapon;
struct FLinearColor;

/**
 * 
 */
UCLASS()
class UNREALPORTFOLIO_API UHKInventoryWidget : public UHKUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void SetAbilitySystemComponent(AActor* InOwner) override;

	UFUNCTION()
	void PutItemInInventory(AHKWeapon* NewWeapon);

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> ItemImage1;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> ItemImage2;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> ItemImage3;
	
	UPROPERTY()
	TArray<TObjectPtr<UImage>> ItemImages;

	int InventorySlotCount = 1;
	FLinearColor EquipColor;
};
