// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"


// Collision
#define CHANNEL_HKCHARACTER ECC_GameTraceChannel1
#define CHANNEL_ATTACK ECC_GameTraceChannel2

// TAG
#define HKTAG_GAMEPLAYCUE_CHARACTER_ATTACKHIT FGameplayTag::RequestGameplayTag(FName("GameplayCue.Character.AttackHit"))
#define HKTAG_GAMEPLAYCUE_CHARACTER_MUZZLEFLASH FGameplayTag::RequestGameplayTag(FName("GameplayCue.Character.MuzzleFlash"))
#define HKTAG_GAMEPLAYCUE_CHARACTER_LAUNCHEREXPLOSION FGameplayTag::RequestGameplayTag(FName("GameplayCue.Character.LauncherExplosion"))
#define HKTAG_GAMEPLAYCUE_CHARACTER_DEAD FGameplayTag::RequestGameplayTag(FName("GameplayCue.Character.Dead"))
#define HKTAG_CHARACTER_STATE_ISZOOM FGameplayTag::RequestGameplayTag(FName("Character.State.IsZoom"))