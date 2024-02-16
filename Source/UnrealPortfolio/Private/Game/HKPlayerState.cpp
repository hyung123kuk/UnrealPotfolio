// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/HKPlayerState.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystem/AttributeSet/HKCharacterAttributeSet.h"

AHKPlayerState::AHKPlayerState()
{
	ASC = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("ASC"));
	ASC->SetIsReplicated(true);
	ASC->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);

	CharacterAttributeSet = CreateDefaultSubobject<UHKCharacterAttributeSet>(TEXT("AttributeSet"));
}

UAbilitySystemComponent* AHKPlayerState::GetAbilitySystemComponent() const
{
	return ASC;
}
