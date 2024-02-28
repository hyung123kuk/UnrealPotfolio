// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/AttributeSet/HKCharacterAttributeSet.h"
#include "GameplayEffectExtension.h"
#include "Net/UnrealNetwork.h"

UHKCharacterAttributeSet::UHKCharacterAttributeSet()
	: MaxHealth(100.f),
	Damage(0.f)
{
	InitHealth(GetMaxHealth());
}

bool UHKCharacterAttributeSet::PreGameplayEffectExecute(FGameplayEffectModCallbackData& Data)
{

	if (!Super::PreGameplayEffectExecute(Data))
	{
		return false;
	}

	return true;
}

void UHKCharacterAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	if (Data.EvaluatedData.Attribute == GetDamageAttribute())
	{
		UE_LOG(LogTemp, Log, TEXT("Damage : %f"), GetDamage());
		SetHealth(FMath::Clamp(GetHealth() - GetDamage(), 0.0f, GetMaxHealth()));
		UE_LOG(LogTemp, Log, TEXT("Health : %f"), GetHealth());
		SetDamage(0.0f);
	}
}

void UHKCharacterAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UHKCharacterAttributeSet, Health, COND_None, REPNOTIFY_OnChanged);
}

void UHKCharacterAttributeSet::OnRep_Health(const FGameplayAttributeData& OldHealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UHKCharacterAttributeSet, Health, OldHealth);
}
