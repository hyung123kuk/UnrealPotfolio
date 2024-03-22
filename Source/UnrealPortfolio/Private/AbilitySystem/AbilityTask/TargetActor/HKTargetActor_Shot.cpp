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

	FHitResult HitResult;
	UCameraComponent* CameraComponent = Character->CameraComponent;
	FVector CameraStartLocation = CameraComponent->GetComponentLocation();
	FVector CameraEndLocation = Character->GetPawnViewLocation() + CameraComponent->GetForwardVector() * 5000.f;

	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);
	QueryParams.AddIgnoredComponent((const UPrimitiveComponent*)(CameraComponent));
	QueryParams.bTraceComplex = true;

	FVector MuzzleLocation;

	if (Character->GetAbilitySystemComponent()->HasMatchingGameplayTag(HKTAG_CHARACTER_STATE_ISZOOM))
	{
		MuzzleLocation = CameraStartLocation;
		Character->Zoom(0);
	}
	else
	{
		MuzzleLocation = Character->GetMesh()->GetSocketLocation("MuzzleFlash");
	}

	bool HitDetected = GetWorld()->LineTraceSingleByChannel(HitResult, MuzzleLocation, CameraEndLocation, CHANNEL_ATTACK, QueryParams);
	
	if (HitDetected == true)
	{
		UE_LOG(LogTemp, Log, TEXT("Hit True"));
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("Hit False"));
	}


	FGameplayAbilityTargetDataHandle DataHandle;
	if (HitDetected)
	{
		FGameplayAbilityTargetData_SingleTargetHit* TargetData = new FGameplayAbilityTargetData_SingleTargetHit(HitResult);
		DataHandle.Add(TargetData);
	}

	DrawDebugLine(GetWorld(), MuzzleLocation, CameraEndLocation, FColor(255, 255, 255, 64), false, 0.1f, 0U, 0.5f);

	return DataHandle;
}
