// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayModMagnitudeCalculation.h"
#include "HKMMC_Ammo.generated.h"

/**
 * 
 */
UCLASS()
class UNREALPORTFOLIO_API UHKMMC_Ammo : public UGameplayModMagnitudeCalculation
{
	GENERATED_BODY()
public:
	//잘못된 사용입니다. AttributeData외의 다른 값을 변경하기 위해 CustomClass를 만들어 사용하였는데,
	//1. 이 함수는 서버에서만 적용됩니다. 따라서 Attribute에 대한 연산이 아니면 클라이언트와 값이 달라집니다.
	//2. AbilitySystem에서 제공하는 GameplayEffect 블루프린트 인터페이스와 맞지 않습니다. 
	//따라서 이 클래스는 더 이상 사용하지 않습니다.

	UHKMMC_Ammo();

	virtual float CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const override;

};
