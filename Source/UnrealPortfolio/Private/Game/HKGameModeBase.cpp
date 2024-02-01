// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/HKGameModeBase.h"
#include "Characters/HKCharacterBase.h"
#include "Controllers/HKPlayerControllerBase.h"

AHKGameModeBase::AHKGameModeBase()
{
	static ConstructorHelpers::FClassFinder<AHKCharacterBase>CharacterRef(TEXT("/Script/Engine.Blueprint'/Game/Main/Blueprints/Characters/BP_PlayerCharacter.BP_PlayerCharacter_C'"));

	if (CharacterRef.Class)
	{
		DefaultPawnClass = CharacterRef.Class;
	}
	
	static ConstructorHelpers::FClassFinder<AHKPlayerControllerBase>ControllerRef(TEXT("/Script/Engine.Blueprint'/Game/Main/Blueprints/Controllers/BP_PlayerController.BP_PlayerController_C'"));

	if (ControllerRef.Class)
	{
		PlayerControllerClass = ControllerRef.Class;
	}


}
