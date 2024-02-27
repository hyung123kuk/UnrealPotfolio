// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "AbilitySystemInterface.h"
#include "HKPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class UNREALPORTFOLIO_API AHKPlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()
	
public:
	AHKPlayerState();
	
//Ability Func
public:
	virtual class UAbilitySystemComponent* GetAbilitySystemComponent() const override;

//Ability Params
protected:
	UPROPERTY(EditAnywhere, Category = "AAGAS")
	TObjectPtr<UAbilitySystemComponent> ASC;
	
	UPROPERTY()
	TObjectPtr<class UHKCharacterAttributeSet> CharacterAttributeSet;
};
