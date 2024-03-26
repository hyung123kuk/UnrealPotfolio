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
	Shotgun,
	Launcher,
};

class USkeletalMesh;

UCLASS()
class UNREALPORTFOLIO_API AHKWeapon : public AActor
{
	GENERATED_BODY()

public:
	AHKWeapon();

	USkeletalMesh& GetWeaponMesh() const { return *WeaponMesh->GetSkeletalMeshAsset(); }
	USkeletalMeshComponent* GetWeaponMeshComponent() const { return WeaponMesh; }
	UAnimMontage& GetShotMontage() const { return *ShotMontage; }
	TSubclassOf<class AHKTargetActor_Shot> GetTargetActor() const { return TargetActor; }

	const FORCEINLINE EWeaponType& GetWeaponType() const { return WeaponType; }
	const FORCEINLINE float GetDamage() const { return Damage; }
	const FORCEINLINE float GetRecoil() const { return Recoil; }
	const FORCEINLINE float GetBulletSpreadAngle() const { return BulletSpreadAngle; }
	const FORCEINLINE float GetMaxDamageRange() const { return MaxDamageRange; }
	const FORCEINLINE int32 GetBulletsPerCartridge() const { return BulletsPerCartridge; }

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AAWeapon")
	float Damage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AAWeapon")
	float Recoil;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AAWeapon")
	float BulletSpreadAngle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AAWeapon")
	float MaxDamageRange;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AAWeapon")
	int32 BulletsPerCartridge;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AAWeapon")
	TObjectPtr<class UAnimMontage> ShotMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AAWeapon")
	TObjectPtr<USkeletalMeshComponent> WeaponMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AAWeapon")
	TSubclassOf<class UHKGameplayAbility_WeaponSwap> SwapAbility;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AAWeapon")
	EWeaponType WeaponType = EWeaponType::MeleeWeapon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AAWeapon")
	TSubclassOf<class AHKTargetActor_Shot> TargetActor;

	friend class AHKPlayerCharacter;
};