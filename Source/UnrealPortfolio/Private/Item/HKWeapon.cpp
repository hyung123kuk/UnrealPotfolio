// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/HKWeapon.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/SceneComponent.h"

AHKWeapon::AHKWeapon()
{
	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
	WeaponMesh->SetupAttachment(GetRootComponent());
	LoadedBullets = MaximumLoadedBullets;
	RemainBullets = MaximumLoadedBullets * 3;
}

bool AHKWeapon::LoadedBullet()
{
	if (WeaponType == EWeaponType::MeleeWeapon)
		return true;

	if (LoadedBullets <= 0)
		return false;

	LoadedBullets--;

	UE_LOG(LogTemp, Log, TEXT("LoadedBullets : %d / RemainBullets %d"), LoadedBullets, RemainBullets);

	return true;
}

void AHKWeapon::Reload()
{
	if (!CanReload())
		return;

	int32 CanLoadedBullets;
	if (RemainBullets >= MaximumLoadedBullets - LoadedBullets)
	{
		CanLoadedBullets = MaximumLoadedBullets - LoadedBullets;
	}
	else
	{
		CanLoadedBullets = RemainBullets;
	}

	RemainBullets -= CanLoadedBullets;
	LoadedBullets += CanLoadedBullets;

	UE_LOG(LogTemp, Log, TEXT("LoadedBullets : %d / RemainBullets %d"), LoadedBullets, RemainBullets);
}

bool AHKWeapon::CanReload()
{
	if (WeaponType == EWeaponType::MeleeWeapon)
		return false;

	if (RemainBullets == 0)
		return false;

	if (LoadedBullets == MaximumLoadedBullets)
		return false;

	return true;
}

void AHKWeapon::AddAmmo()
{
	RemainBullets += MaximumLoadedBullets * 2;
	UE_LOG(LogTemp, Log, TEXT("LoadedBullets : %d / RemainBullets : %d"), LoadedBullets, RemainBullets);
}

