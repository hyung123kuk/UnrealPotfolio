// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/HKWeapon.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/SceneComponent.h"

AHKWeapon::AHKWeapon()
{
	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
	WeaponMesh->SetupAttachment(GetRootComponent());
}

