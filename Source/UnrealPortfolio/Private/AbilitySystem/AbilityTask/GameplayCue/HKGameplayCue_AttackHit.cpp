// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/AbilityTask/GameplayCue/HKGameplayCue_AttackHit.h"
#include "Particles/ParticleSystem.h"
#include "Kismet/GameplayStatics.h"

UHKGameplayCue_AttackHit::UHKGameplayCue_AttackHit()
{
	static ConstructorHelpers::FObjectFinder<UParticleSystem> ExplosionRef(TEXT("/Script/Engine.ParticleSystem'/Game/Main/Effect/P_TwinBlast_Primary_HitCharacter.P_TwinBlast_Primary_HitCharacter'"));
	if (ExplosionRef.Object)
	{
		ParticleSystem = ExplosionRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<USoundWave> HitSoundRef(TEXT("/Script/Engine.SoundWave'/Game/Main/Effect/Rifle_ImpactBody04.Rifle_ImpactBody04'"));
	if (HitSoundRef.Succeeded())
	{
		HitSound = HitSoundRef.Object;
	}

}

bool UHKGameplayCue_AttackHit::OnExecute_Implementation(AActor* Target, const FGameplayCueParameters& Parameters) const
{
	const FHitResult* HitResult = Parameters.EffectContext.GetHitResult();
	if (HitResult)
	{
		UGameplayStatics::SpawnEmitterAtLocation(Target, ParticleSystem, HitResult->ImpactPoint, FRotator::ZeroRotator, true);
		UGameplayStatics::PlaySound2D(this, HitSound);

		return true;
	}

	return false;
}
