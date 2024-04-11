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
	//�߸��� ����Դϴ�. AttributeData���� �ٸ� ���� �����ϱ� ���� CustomClass�� ����� ����Ͽ��µ�,
	//1. �� �Լ��� ���������� ����˴ϴ�. ���� Attribute�� ���� ������ �ƴϸ� Ŭ���̾�Ʈ�� ���� �޶����ϴ�.
	//2. AbilitySystem���� �����ϴ� GameplayEffect �������Ʈ �������̽��� ���� �ʽ��ϴ�. 
	//���� �� Ŭ������ �� �̻� ������� �ʽ��ϴ�.

	UHKMMC_Ammo();

	virtual float CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const override;

};
