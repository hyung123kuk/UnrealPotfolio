// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/HKCharacterBase.h"

// Sets default values
AHKCharacterBase::AHKCharacterBase()
{

	ConstructorHelpers::FObjectFinder<USkeletalMesh> CharacterMeshRef(TEXT("/Script/Engine.SkeletalMesh'/Game/Main/Mesh/TwinBlast_ShadowOps.TwinBlast_ShadowOps'"));
	if (CharacterMeshRef.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(CharacterMeshRef.Object);
	}

	ConstructorHelpers::FObjectFinder<UAnimMontage> ShotMontageRef(TEXT("/Script/Engine.AnimMontage'/Game/Main/Animation/Montage/AM_Shot.AM_Shot'"));
	if (ShotMontageRef.Succeeded())
	{
		ShotMontage = ShotMontageRef.Object;
	}

	ConstructorHelpers::FObjectFinder<UAnimMontage> DeadMontageRef(TEXT("/Script/Engine.AnimMontage'/Game/Main/Animation/Montage/AM_Dead.AM_Dead'"));
	if (DeadMontageRef.Succeeded())
	{
		DeadMontage = DeadMontageRef.Object;
	}


}


