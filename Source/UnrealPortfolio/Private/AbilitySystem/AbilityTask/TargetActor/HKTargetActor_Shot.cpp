// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/AbilityTask/TargetActor/HKTargetActor_Shot.h"
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

AHKTargetActor_Shot::AHKTargetActor_Shot()
{
	bMadeLocal = true;
}

void AHKTargetActor_Shot::StartTargeting(UGameplayAbility* Ability)
{
	Super::StartTargeting(Ability);
	ConfirmTargeting();
}

FGameplayAbilityTargetDataHandle AHKTargetActor_Shot::MakeTargetData() const
{

	FVector ShotStartLocation;
	//스나이퍼 줌일때 공격 시작점은 카메라 시작점으로
	if (PlayerCharacter->GetAbilitySystemComponent()->HasMatchingGameplayTag(HKTAG_CHARACTER_STATE_ISZOOM))
	{
		ShotStartLocation = CameraLocation;
		PlayerCharacter->Zoom(0);
	}
	else
	{
		ShotStartLocation = MuzzleLocation;
	}

	FGameplayAbilityTargetDataHandle DataHandle;
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(SourceActor);
	QueryParams.bTraceComplex = true;

	for (int32 BulletIndex = 0; BulletIndex < Weapon->GetBulletsPerCartridge(); ++BulletIndex)
	{
		FHitResult HitResult;
		const FVector RandPos = UKismetMathLibrary::RandomUnitVectorInConeInDegrees(CameraComponent->GetForwardVector(), Weapon->GetBulletSpreadAngle());
		const FVector BulletEndLocation = PlayerCharacter->GetPawnViewLocation() + (CameraComponent->GetForwardVector() + RandPos) * Weapon->GetDamageRange();
		const bool HitDetected = GetWorld()->LineTraceSingleByChannel(HitResult, ShotStartLocation, BulletEndLocation, CHANNEL_ATTACK, QueryParams);

		if (HitDetected)
		{
			FGameplayAbilityTargetData_SingleTargetHit* TargetData = new FGameplayAbilityTargetData_SingleTargetHit(HitResult);
			DataHandle.Add(TargetData);
		}

#pragma region ENABLE_DRAW_DEBUG

		DrawDebugLine(GetWorld(), MuzzleLocation, BulletEndLocation, FColor(255, 255, 255, 64), false, 0.1f, 0U, 0.5f);
		if (HitDetected == true)
		{
			UE_LOG(LogTemp, Log, TEXT("%s"), *HitResult.BoneName.ToString());
			DrawDebugSphere(GetWorld(), HitResult.Location, 3.f, 16, FColor(255, 0, 0, 255), true, 20.f, 0U, 5.f);
		}
#pragma endregion

	}

	return DataHandle;
}
