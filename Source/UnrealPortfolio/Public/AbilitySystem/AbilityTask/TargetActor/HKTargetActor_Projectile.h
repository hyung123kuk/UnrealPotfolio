// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/AbilityTask/TargetActor/HKTargetActorBase.h"
#include "HKTargetActor_Projectile.generated.h"

/**
 * 
 */
UCLASS()
class UNREALPORTFOLIO_API AHKTargetActor_Projectile : public AHKTargetActorBase
{
	GENERATED_BODY()
	
public:
	AHKTargetActor_Projectile();
	virtual void StartTargeting(UGameplayAbility* Ability) override;

	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit);

protected:
	virtual FGameplayAbilityTargetDataHandle MakeTargetData() const override;

	UPROPERTY(VisibleDefaultsOnly, Category = Projectile)
	TObjectPtr<class USphereComponent> CollisionComponent;

	UPROPERTY(VisibleAnywhere, Category = Movement)
	TObjectPtr<class UProjectileMovementComponent> ProjectileMovementComponent;
};
