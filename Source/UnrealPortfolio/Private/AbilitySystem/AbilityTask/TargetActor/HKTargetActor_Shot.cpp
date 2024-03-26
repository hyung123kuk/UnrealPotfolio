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

AHKTargetActor_Shot::AHKTargetActor_Shot()
{
}

void AHKTargetActor_Shot::StartTargeting(UGameplayAbility* Ability)
{
	Super::StartTargeting(Ability);
	SourceActor = Ability->GetCurrentActorInfo()->AvatarActor.Get();
}

void AHKTargetActor_Shot::ConfirmTargetingAndContinue()
{
	if (SourceActor)
	{
		FGameplayAbilityTargetDataHandle DataHandle = MakeTargetData();
		TargetDataReadyDelegate.Broadcast(DataHandle);
	}
}

FGameplayAbilityTargetDataHandle AHKTargetActor_Shot::MakeTargetData() const
{
	AHKPlayerCharacter* Character = CastChecked<AHKPlayerCharacter>(SourceActor);

	FGameplayAbilityTargetDataHandle DataHandle;
	FVector MuzzleLocation;

	const UCameraComponent* CameraComponent = Character->CameraComponent;
	const AHKWeapon* Weapon = Character->GetWeapon();

	const FVector CameraStartLocation = CameraComponent->GetComponentLocation();

	//스나이퍼 줌일때 공격 시작점은 카메라 시작점으로
	if (Character->GetAbilitySystemComponent()->HasMatchingGameplayTag(HKTAG_CHARACTER_STATE_ISZOOM))
	{
		MuzzleLocation = CameraStartLocation;
		Character->Zoom(0);
	}
	else
	{
		MuzzleLocation = Character->GetMesh()->GetSocketLocation("MuzzleFlash");
	}

	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(SourceActor);
	QueryParams.bTraceComplex = true;

	for (int32 BulletIndex = 0; BulletIndex < Weapon->GetBulletsPerCartridge(); ++BulletIndex)
	{
		FHitResult HitResult;
		const FVector RandPos = UKismetMathLibrary::RandomUnitVectorInConeInDegrees(CameraComponent->GetForwardVector(), Weapon->GetBulletSpreadAngle());
		const FVector BulletEndLocation = Character->GetPawnViewLocation() + (CameraComponent->GetForwardVector() + RandPos) * Weapon->GetMaxDamageRange();
		const bool HitDetected = GetWorld()->LineTraceSingleByChannel(HitResult, MuzzleLocation, BulletEndLocation, CHANNEL_ATTACK, QueryParams);

		if (HitDetected)
		{
			FGameplayAbilityTargetData_SingleTargetHit* TargetData = new FGameplayAbilityTargetData_SingleTargetHit(HitResult);
			DataHandle.Add(TargetData);
		}

		#pragma region Debug

		DrawDebugLine(GetWorld(), MuzzleLocation, BulletEndLocation, FColor(255, 255, 255, 64), false, 0.1f, 0U, 0.5f);
		if (HitDetected == true)
		{
			UE_LOG(LogTemp, Log, TEXT("Hit True"));
		}
		else
		{
			UE_LOG(LogTemp, Log, TEXT("Hit False"));
		}

		#pragma endregion

	}

	return DataHandle;
}
