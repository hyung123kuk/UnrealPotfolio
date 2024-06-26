// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "HKCharacterAttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDeadDelegate,UAbilitySystemComponent* , AbilitySystemComponent);


/**
 * 
 */
UCLASS()
class UNREALPORTFOLIO_API UHKCharacterAttributeSet : public UAttributeSet
{
	GENERATED_BODY()
	
public:
	UHKCharacterAttributeSet();
	ATTRIBUTE_ACCESSORS(UHKCharacterAttributeSet, Health);
	ATTRIBUTE_ACCESSORS(UHKCharacterAttributeSet, MaxHealth);
	ATTRIBUTE_ACCESSORS(UHKCharacterAttributeSet, Damage);

	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	virtual void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data) override;

	mutable FDeadDelegate DeadDelegate;

private:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION()
	virtual void OnRep_Health(const FGameplayAttributeData& OldHealth);

protected:
	UPROPERTY(ReplicatedUsing = OnRep_Health, BlueprintReadOnly, Category = "AAHealth", Meta = (AllowPrivateAccess = "true"))
	FGameplayAttributeData Health;
	UPROPERTY(BlueprintReadOnly, Category = "AAHealth", Meta = (AllowPrivateAccess = "true"))
	FGameplayAttributeData MaxHealth;
	UPROPERTY(BlueprintReadOnly, Category = "AAAttack", Meta = (AllowPrivateAccess = "true"))
	FGameplayAttributeData Damage;

	bool isDead = false;
};
