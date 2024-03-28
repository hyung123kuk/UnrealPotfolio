// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/HKWeapon.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/SceneComponent.h"

AHKWeapon::AHKWeapon()
{
	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
	WeaponMesh->SetupAttachment(GetRootComponent());
}

bool AHKWeapon::LoadedBullet()
{
	if (LoadedBullets <= 0)
		return false;

	LoadedBullets--;

	UE_LOG(LogTemp, Log, TEXT("Bullet : %d / %d"), LoadedBullets, RemainBullets);

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

	UE_LOG(LogTemp, Log, TEXT("Remain Bullet : %d / MaximumLodedBullet %d"), LoadedBullets, RemainBullets);
}

bool AHKWeapon::CanReload()
{
	if (RemainBullets == 0)
		return false;

	if (LoadedBullets == MaximumLoadedBullets)
		return false;

	return true;
}

