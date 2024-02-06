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

}


