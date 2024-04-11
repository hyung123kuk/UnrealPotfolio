// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HKInventoryWidget.h"
#include "Characters/HKPlayerCharacter.h"
#include "Item/HKWeapon.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"


void UHKInventoryWidget::SetAbilitySystemComponent(AActor* InOwner)
{
	Super::SetAbilitySystemComponent(InOwner);

	if (ASC)
	{
		ItemImages.Add(ItemImage1);
		ItemImages.Add(ItemImage2);
		ItemImages.Add(ItemImage3);

		EquipColor = FLinearColor(1.f, 1.f, 1.f, 1.f);
		AHKPlayerCharacter* PlayerCharacter = Cast<AHKPlayerCharacter>(InOwner);
		if (PlayerCharacter)
		{
			PlayerCharacter->GetWeaponDelegate.AddDynamic(this, &UHKInventoryWidget::PutItemInInventory);
		}
	}
}

void UHKInventoryWidget::PutItemInInventory(AHKWeapon* NewWeapon)
{
	UTexture2D* ItemTexture = &NewWeapon->GetWeaponTexture();
	ItemImages[InventorySlotCount]->SetBrushFromTexture(ItemTexture);
	ItemImages[InventorySlotCount]->SetColorAndOpacity(EquipColor);

	InventorySlotCount++;

	NewWeapon->ChangeBulletDelegate.AddDynamic(this, &UHKInventoryWidget::SetBullet);
}

void UHKInventoryWidget::SetBullet(int NowBullet, int MaxBullet)
{
	BulletText->SetText(FText::FromString(FString::Printf(TEXT("%d/%d"), NowBullet, MaxBullet)));
}


