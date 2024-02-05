// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/HKCharacterBase.h"

// Sets default values
AHKCharacterBase::AHKCharacterBase()
{

	ConstructorHelpers::FObjectFinder<USkeletalMesh> CharacterMeshRef(TEXT("/Script/Engine.SkeletalMesh'/Game/Main/Meshs/TwinBlast_ShadowOps.TwinBlast_ShadowOps'"));
	if (CharacterMeshRef.Object)
	{
		GetMesh()->SetSkeletalMesh(CharacterMeshRef.Object);
	}

}


