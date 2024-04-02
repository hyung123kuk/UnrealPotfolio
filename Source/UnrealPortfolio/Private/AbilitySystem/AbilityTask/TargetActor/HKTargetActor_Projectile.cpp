// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/AbilityTask/TargetActor/HKTargetActor_Projectile.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Characters/HKPlayerCharacter.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Components/CapsuleComponent.h"
#include "Camera/CameraComponent.h"
#include "UnrealPortfolio/UnrealPortfolio.h"
#include "Item/HKWeapon.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"

AHKTargetActor_Projectile::AHKTargetActor_Projectile()
{
    PrimaryActorTick.bCanEverTick = true;

    CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	CollisionComponent->OnComponentHit.AddDynamic(this, &AHKTargetActor_Projectile::OnHit);
	CollisionComponent->BodyInstance.SetCollisionProfileName(TEXT("Projectile"));
	RootComponent = CollisionComponent;

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
	ProjectileMovementComponent->SetUpdatedComponent(CollisionComponent);
	ProjectileMovementComponent->bRotationFollowsVelocity = true;

    CollisionComponent->InitSphereRadius(15.0f);
    ProjectileMovementComponent->Bounciness = 0.3f;
    ProjectileMovementComponent->InitialSpeed = 5000.0f;
    ProjectileMovementComponent->MaxSpeed = 5000.0f;
    ProjectileMovementComponent->ProjectileGravityScale = 5.f;
    bMadeLocal = false;
}

void AHKTargetActor_Projectile::StartTargeting(UGameplayAbility* Ability)
{
    Super::StartTargeting(Ability);
}

void AHKTargetActor_Projectile::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{
    ConfirmTargeting(); 
}

FGameplayAbilityTargetDataHandle AHKTargetActor_Projectile::MakeTargetData() const
{
	TArray<FOverlapResult> Overlaps;
	AHKPlayerCharacter* ShooterCharacter = CastChecked<AHKPlayerCharacter>(SourceActor);
	const float DamageRange = ShooterCharacter->GetWeapon()->GetDamageRange();

	FVector Origin = GetActorLocation();
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(SourceActor);
	QueryParams.bTraceComplex = true;
	GetWorld()->OverlapMultiByChannel(Overlaps, Origin, FQuat::Identity, CHANNEL_ATTACK, FCollisionShape::MakeSphere(DamageRange), QueryParams);

	TArray<TWeakObjectPtr<AActor>> HitActors;
	for (const FOverlapResult& Overlap : Overlaps)
	{
		AActor* HitActor = Overlap.OverlapObjectHandle.FetchActor<AActor>();
		if (HitActor && !HitActors.Contains(HitActor))
		{
			HitActors.Add(HitActor);
		}

	}

	FGameplayAbilityTargetData_ActorArray* ActorsData = new FGameplayAbilityTargetData_ActorArray();
	ActorsData->SetActors(HitActors);

#if ENABLE_DRAW_DEBUG
	FColor DrawColor = HitActors.Num() > 0 ? FColor::Green : FColor::Red;
	DrawDebugSphere(GetWorld(), Origin, DamageRange, 16, DrawColor, false, 3.0f);
#endif

	FGameplayCueParameters CueParam;
	CueParam.Location = Origin;
	ShooterCharacter->GetAbilitySystemComponent()->ExecuteGameplayCue(HKTAG_GAMEPLAYCUE_CHARACTER_LAUNCHEREXPLOSION, CueParam);

	return FGameplayAbilityTargetDataHandle(ActorsData);
}
