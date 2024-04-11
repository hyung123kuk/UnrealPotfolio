// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SkeletalMeshComponent.h"
#include "HKItem.h"
#include "Engine/DataTable.h"
#include "GameplayTagContainer.h"
#include "HKWeapon.generated.h"

class USkeletalMesh;
class UTexture2D;

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

USTRUCT(BlueprintType)
struct FWeaponRow : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayTag MessageTag = FGameplayTag();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<AHKWeapon> WeaponClass;
};



UCLASS()
class UNREALPORTFOLIO_API AHKWeapon : public AHKItem
{
	GENERATED_BODY()

public:
	AHKWeapon();
	bool LoadedBullet();
	void Reload();
	bool CanReload();
	void AddAmmo();

	USkeletalMesh& GetWeaponMesh() const { return *WeaponMesh->GetSkeletalMeshAsset(); }
	UAnimMontage& GetShotMontage() const { return *ShotMontage; }
	UAnimMontage& GetReloadMontage() const { return *ReloadMontage; }
	UAnimMontage& GetSwapMontage() const { return *SwapMontage; }
	UTexture2D& GetWeaponTexture() const { return *WeaponTexture; }
	TSubclassOf<class AHKTargetActorBase> GetTargetActor() const { return TargetActor; }
	TSubclassOf<class UHKGameplayAbility_WeaponSwap> GetSwapAbility() const { return SwapAbility; }

	const FORCEINLINE EWeaponType& GetWeaponType() const { return WeaponType; }
	const FORCEINLINE float GetDamage() const { return Damage; }
	const FORCEINLINE float GetRecoil() const { return Recoil; }
	const FORCEINLINE float GetBulletSpreadAngle() const { return BulletSpreadAngle; }
	const FORCEINLINE float GetDamageRange() const { return DamageRange; }
	const FORCEINLINE int32 GetBulletsPerCartridge() const { return BulletsPerCartridge; }
	const FORCEINLINE int32 GetLoadedBullets() const { return LoadedBullets; }
	const FORCEINLINE int32 GetMaximumLoadedBullets() const { return MaximumLoadedBullets; }
	const FORCEINLINE int32 GetRemainBullets() const { return RemainBullets; }
	const FORCEINLINE float GetBeforeZoomFieldOfView() const { return BeforeZoomFieldOfView; }
	const FORCEINLINE float GetAfterZoomFieldOfView() const { return AfterZoomFieldOfView; }
	const FORCEINLINE float GetBeforeZoomTargetArmLength() const { return BeforeZoomTargetArmLength; }
	const FORCEINLINE float GetAfterZoomTargetArmLength() const { return AfterZoomTargetArmLength; }

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AAWeapon")
	TObjectPtr<UTexture2D> WeaponTexture;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AAWeapon")
	float Damage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AAWeapon")
	float Recoil;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AAWeapon")
	float BulletSpreadAngle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AAWeapon")
	float DamageRange;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AAWeapon")
	int32 BulletsPerCartridge;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AAWeapon")
	int32 LoadedBullets;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AAWeapon")
	int32 MaximumLoadedBullets;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AAWeapon")
	int32 RemainBullets;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AAWeapon")
	float BeforeZoomFieldOfView;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AAWeapon")
	float AfterZoomFieldOfView;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AAWeapon")
	float BeforeZoomTargetArmLength;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AAWeapon")
	float AfterZoomTargetArmLength;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AAWeapon")
	TObjectPtr<class UAnimMontage> ShotMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AAWeapon")
	TObjectPtr<class UAnimMontage> ReloadMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AAWeapon")
	TObjectPtr<class UAnimMontage> SwapMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AAWeapon")
	TObjectPtr<USkeletalMeshComponent> WeaponMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AAWeapon")
	TSubclassOf<class UHKGameplayAbility_WeaponSwap> SwapAbility;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AAWeapon")
	EWeaponType WeaponType = EWeaponType::MeleeWeapon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AAWeapon")
	TSubclassOf<class AHKTargetActorBase> TargetActor;

};