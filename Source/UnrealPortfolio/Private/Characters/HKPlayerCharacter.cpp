// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/HKPlayerCharacter.h"
#include "GameframeWork/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "InputMappingContext.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Animation/HKAnimInstance.h"
#include "Components/SkeletalMeshComponent.h"
#include "UnrealPortfolio/UnrealPortfolio.h"
#include "Net/UnrealNetwork.h"
#include "Engine/Engine.h"
#include "AbilitySystemComponent.h"
#include "Game/HKPlayerState.h"
#include "Item/HKWeapon.h"
#include "AbilitySystem/Abilities/HKGameplayAbility_WeaponSwap.h"

AHKPlayerCharacter::AHKPlayerCharacter()
{
	bReplicates = true;

	PrimaryActorTick.bCanEverTick = true;

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	SpringArmComponent->SetupAttachment(RootComponent);
	SpringArmComponent->TargetArmLength = 400.0f;
	SpringArmComponent->bUsePawnControlRotation = true;
	SpringArmComponent->bDoCollisionTest = true;
	SpringArmComponent->bInheritPitch = true;
	SpringArmComponent->bInheritYaw = true;
	SpringArmComponent->bInheritRoll = false;
	
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetupAttachment(SpringArmComponent);
	CameraComponent->SetRelativeLocation(FVector(0.f, 60.f, 10.f));
	CameraComponent->bUsePawnControlRotation = false;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = false;
	GetCharacterMovement()->bUseControllerDesiredRotation = true;

	ConstructorHelpers::FObjectFinder<UInputMappingContext> InputMappingContextRef(TEXT("/Script/EnhancedInput.InputMappingContext'/Game/Main/Input/IMC_Gun.IMC_Gun'"));
	if (InputMappingContextRef.Succeeded())
	{
		DefaultMappingContext = InputMappingContextRef.Object;
	}

	ConstructorHelpers::FObjectFinder<UInputAction> MoveActionRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Main/Input/InputAction/IA_Move.IA_Move'"));
	if (MoveActionRef.Succeeded())
	{
		MoveAction = MoveActionRef.Object;
	}

	ConstructorHelpers::FObjectFinder<UInputAction> LookActionRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Main/Input/InputAction/IA_Look.IA_Look'"));
	if (LookActionRef.Succeeded())
	{
		LookAction = LookActionRef.Object;
	}

	ConstructorHelpers::FObjectFinder<UInputAction> JumpActionRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Main/Input/InputAction/IA_Jump.IA_Jump'"));
	if (JumpActionRef.Succeeded())
	{
		JumpAction = JumpActionRef.Object;
	}

	ConstructorHelpers::FObjectFinder<UInputAction> AttackActionRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Main/Input/InputAction/IA_Shot.IA_Shot'"));
	if (AttackActionRef.Succeeded())
	{
		AttackAction = AttackActionRef.Object;
	}

	ConstructorHelpers::FObjectFinder<UInputAction> ChangeWeaponActionRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Main/Input/InputAction/IA_ChangeWeapon.IA_ChangeWeapon'"));
	if (ChangeWeaponActionRef.Succeeded())
	{
		ChangeWeaponAction = ChangeWeaponActionRef.Object;
	}

	ConstructorHelpers::FObjectFinder<UInputAction> ZoomActionRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Main/Input/InputAction/IA_Zoom.IA_Zoom'"));
	if (ZoomActionRef.Succeeded())
	{
		ZoomAction = ZoomActionRef.Object;
	}

	ConstructorHelpers::FClassFinder<UHKAnimInstance> AnimInstanceRef(TEXT("/Script/Engine.Blueprint'/Game/Main/Blueprints/Animation/BP_AnimInstance.BP_AnimInstance_C'"));
	if (AnimInstanceRef.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(AnimInstanceRef.Class);
	}
	WeaponSocket_R = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Weapon_R"));
	WeaponSocket_R->SetupAttachment(GetMesh(), TEXT("WeaponSocket_R"));
	WeaponNum = 0;
}

void AHKPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	APlayerController* PlayerController = Cast<APlayerController>(GetController());

	if (IsValid(PlayerController) == true)
	{
		UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
		if (IsValid(Subsystem) == true)
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}

void AHKPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);

	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AHKPlayerCharacter::Move);
	EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ThisClass::Look);
	EnhancedInputComponent->BindAction(ZoomAction, ETriggerEvent::Started, this, &ThisClass::Zoom);

	if (IsValid(ASC))
	{
		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Triggered, this, &AHKPlayerCharacter::GASInputPressed, 1);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &AHKPlayerCharacter::GASInputPressed, 2);		
		EnhancedInputComponent->BindAction(ChangeWeaponAction, ETriggerEvent::Triggered, this, &AHKPlayerCharacter::ChangeWeapon);
	}

	ChangeWeapon(1);
}

void AHKPlayerCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, InputMoveValue);
	DOREPLIFETIME(ThisClass, InputLookValue);
	DOREPLIFETIME(ThisClass, WeaponNum);
}

void AHKPlayerCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	InitAbilityActorInfo();

	for (const auto& StartAbility : StartAbilities)
	{
		FGameplayAbilitySpec StartSpec(StartAbility);
		ASC->GiveAbility(StartSpec);
	}

	for (const auto& StartInputAbility : StartInputAbilities)
	{
		FGameplayAbilitySpec StartSpec(StartInputAbility.Value);
		StartSpec.InputID = StartInputAbility.Key;
		ASC->GiveAbility(StartSpec);
	}

	for (const auto& SwapWeapon : SwapWeapons)
	{
		auto Swap = SwapWeapon.Value;
		
		FGameplayAbilitySpec StartSpec(Cast<AHKWeapon>(Swap->GetDefaultObject())->SwapAbility);
		ASC->GiveAbility(StartSpec);
	}
}

void AHKPlayerCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState(); 

	InitAbilityActorInfo();

	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if (PlayerController)
	{
		PlayerController->ConsoleCommand(TEXT("showdebug abilitysystem"));
	}
}

void AHKPlayerCharacter::InitAbilityActorInfo()
{
	AHKPlayerState* GASPS = GetPlayerState<AHKPlayerState>();
	check(GASPS);
	ASC = GASPS->GetAbilitySystemComponent();
	ASC->InitAbilityActorInfo(GASPS, this);
	CharacterAttributeSet = GASPS->GetAttributeSet();
}

UAbilitySystemComponent* AHKPlayerCharacter::GetAbilitySystemComponent() const
{
	return ASC;
}

void AHKPlayerCharacter::GASInputPressed(int32 InputId)
{
	FGameplayAbilitySpec* Spec = ASC->FindAbilitySpecFromInputID(InputId);
	if (Spec)
	{
		Spec->InputPressed = true;
		if (Spec->IsActive())
		{
			ASC->AbilitySpecInputPressed(*Spec);
		}
		else
		{
			ASC->TryActivateAbility(Spec->Handle);
		}
	}
}

void AHKPlayerCharacter::GASInputReleased(int32 InputId)
{
	FGameplayAbilitySpec* Spec = ASC->FindAbilitySpecFromInputID(InputId);
	if (Spec)
	{
		Spec->InputPressed = false;
		if (Spec->IsActive())
		{
			ASC->AbilitySpecInputReleased(*Spec);
		}

	}
}

void AHKPlayerCharacter::Move(const FInputActionValue& Value)
{
	InputMoveValue = Value.Get<FVector2D>();

	const FRotator CurrentControlRotation = GetController()->GetControlRotation();
	const FRotator CurrentControlRotationYaw(0.f, CurrentControlRotation.Yaw, 0.f);

	FVector ForwardDirection = FRotationMatrix(CurrentControlRotationYaw).GetUnitAxis(EAxis::X);
	FVector RightDirection = FRotationMatrix(CurrentControlRotationYaw).GetUnitAxis(EAxis::Y);

	AddMovementInput(ForwardDirection, InputMoveValue.X);
	AddMovementInput(RightDirection, InputMoveValue.Y);

	UpdateInputMoveValue_Server(InputMoveValue);
}

void AHKPlayerCharacter::Look(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	AddControllerYawInput(LookAxisVector.X);
	AddControllerPitchInput(LookAxisVector.Y);

	InputLookValue = GetControlRotation();
	UpdateInputLookValue_Server(InputLookValue);
}

void AHKPlayerCharacter::Zoom(const FInputActionValue& Value)
{

	if (!(EquipWeapon->GetWeaponType() == EWeaponType::Sniper))
		return;

	if (ASC->HasMatchingGameplayTag(HKTAG_CHARACTER_STATE_ISZOOM))
	{
		SpringArmComponent->TargetArmLength = 400.0f;
		CameraComponent->FieldOfView = 90.f;
		ASC->RemoveLooseGameplayTag(HKTAG_CHARACTER_STATE_ISZOOM);
	}
	else
	{
		SpringArmComponent->TargetArmLength = 0.f;
		CameraComponent->FieldOfView = 40.f;
		ASC->AddLooseGameplayTag(HKTAG_CHARACTER_STATE_ISZOOM);
	}
	
}

void AHKPlayerCharacter::ChangeWeapon(const FInputActionValue& Value)
{
	int32 InputKey = static_cast<int32>(Value.Get<float>());

	if (WeaponNum == InputKey)
		return;

	if (!ASC)
		return;

	AHKWeapon* NewWeapon = Cast<AHKWeapon>(SwapWeapons.Find(InputKey)->GetDefaultObject());

	if (ASC->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag(FName("Character.State.IsShot"))))
		return;

	FGameplayAbilitySpec* Spec = ASC->FindAbilitySpecFromClass(NewWeapon->SwapAbility);
	UpdateChangeWeapon_Server(InputKey);

	if (Spec)
	{
		if (Spec->IsActive())
		{
			ASC->AbilitySpecInputPressed(*Spec);
		}
		else
		{
			ASC->TryActivateAbility(Spec->Handle);
		}

	}
}

void AHKPlayerCharacter::OnRep_WeaponNum()
{
	EquipWeapon = Cast<AHKWeapon>(SwapWeapons.Find(WeaponNum)->GetDefaultObject());
	WeaponSocket_R->SetSkeletalMesh(&EquipWeapon->GetWeaponMesh());
	SetShotMontage(&EquipWeapon->GetShotMontage());
}

void AHKPlayerCharacter::UpdateChangeWeapon_Server_Implementation(const int32& InputKey)
{
	WeaponNum = InputKey;
	EquipWeapon = Cast<AHKWeapon>(SwapWeapons.Find(WeaponNum)->GetDefaultObject());
	SetShotMontage(&EquipWeapon->GetShotMontage());
}

void AHKPlayerCharacter::UpdateInputMoveValue_Server_Implementation(const FVector2D& OwnerInputMoveValue)
{
	InputMoveValue.X = OwnerInputMoveValue.X;
	InputMoveValue.Y = OwnerInputMoveValue.Y;
}

void AHKPlayerCharacter::UpdateInputLookValue_Server_Implementation(const FRotator& OwnerInputLookValue)
{
	InputLookValue = OwnerInputLookValue;
}

