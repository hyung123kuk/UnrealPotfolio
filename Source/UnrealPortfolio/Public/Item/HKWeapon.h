// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SkeletalMeshComponent.h"
#include "HKWeapon.generated.h"

UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	MeleeWeapon,
	Pistol,
	Rifle,
	Sniper,
};

class USkeletalMesh;

UCLASS()
class UNREALPORTFOLIO_API AHKWeapon : public AActor
{
	GENERATED_BODY()

public:
	AHKWeapon();

	USkeletalMesh& GetWeaponMesh() const { return *WeaponMesh->GetSkeletalMeshAsset(); }
	UAnimMontage& GetShotMontage() const { return *ShotMontage; }
	const FORCEINLINE EWeaponType& GetWeaponType() const { return WeaponType; }

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AAWeapon")
	float Damage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AAWeapon")
	float AttackSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AAWeapon")
	TObjectPtr<class UAnimMontage> ShotMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AAWeapon")
	TObjectPtr<USkeletalMeshComponent> WeaponMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AAWeapon")
	TSubclassOf<class UHKGameplayAbility_WeaponSwap> SwapAbility;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AAWeapon")
	EWeaponType WeaponType = EWeaponType::MeleeWeapon;

	friend class AHKPlayerCharacter;
};