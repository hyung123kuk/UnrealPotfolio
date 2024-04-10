// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "AbilitySystemInterface.h"
#include "HKUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class UNREALPORTFOLIO_API UHKUserWidget : public UUserWidget, public IAbilitySystemInterface
{
	GENERATED_BODY()
	
public:
	virtual void SetAbilitySystemComponent(AActor* InOwner);
	virtual class UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	
protected:
	UPROPERTY(EditAnywhere, Category = "AAGAS")
	TObjectPtr<UAbilitySystemComponent> ASC;

};
