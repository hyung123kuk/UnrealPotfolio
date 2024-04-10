// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "HKWidgetComponent.generated.h"

/**
 * 
 */
UCLASS()
class UNREALPORTFOLIO_API UHKWidgetComponent : public UWidgetComponent
{
	GENERATED_BODY()
	
protected:
	virtual void InitWidget() override;
	
};
