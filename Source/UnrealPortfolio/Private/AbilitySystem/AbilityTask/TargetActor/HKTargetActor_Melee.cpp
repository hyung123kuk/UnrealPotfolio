// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/AbilityTask/TargetActor/HKTargetActor_Melee.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Characters/HKPlayerCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Camera/CameraComponent.h"
#include "UnrealPortfolio/UnrealPortfolio.h"
#include "Item/HKWeapon.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"

AHKTargetActor_Melee::AHKTargetActor_Melee()
{
	bMadeLocal = true;
}

void AHKTargetActor_Melee::StartTargeting(UGameplayAbility* Ability)
{
	Super::StartTargeting(Ability);
	ConfirmTargeting();
}

FGameplayAbilityTargetDataHandle AHKTargetActor_Melee::MakeTargetData() const
{
	FVector MeleeStartLocation = MuzzleLocation;

	FGameplayAbilityTargetDataHandle DataHandle;
	FHitResult HitResult;

	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(SourceActor);
	QueryParams.bTraceComplex = true;

	bool HitDetected = GetWorld()->SweepSingleByChannel(
		HitResult,
		MeleeStartLocation,
		CameraComponent->GetForwardVector() * Weapon->GetDamageRange(),
		FQuat::Identity,
		CHANNEL_ATTACK,
		FCollisionShape::MakeSphere(AttackRadius),
		QueryParams
	);

	if (HitDetected)
	{
		FGameplayAbilityTargetData_SingleTargetHit* TargetData = new FGameplayAbilityTargetData_SingleTargetHit(HitResult);
		DataHandle.Add(TargetData);
	}

#pragma region ENABLE_DRAW_DEBUG

	FVector TraceVec = CameraComponent->GetForwardVector() * Weapon->GetDamageRange();
	FVector Center = MeleeStartLocation + TraceVec * 0.5f;
	float HalfHeight = Weapon->GetDamageRange() * 0.5f + AttackRadius;
	FQuat CapsuleRot = FRotationMatrix::MakeFromZ(TraceVec).ToQuat();
	FColor DrawColor = true == HitDetected ? FColor::Green : FColor::Red;
	float DebugLifeTime = 3.f;

	DrawDebugCapsule(
		GetWorld(),
		Center,
		HalfHeight,
		AttackRadius,
		CapsuleRot,
		DrawColor,
		false,
		DebugLifeTime
	);

#pragma endregion


	return DataHandle;
}
