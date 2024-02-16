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
	CameraComponent->SetRelativeLocation(FVector(0.f, 60.f, 20.f));
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
	if (InputMappingContextRef.Succeeded())
	{
		MoveAction = MoveActionRef.Object;
	}

	ConstructorHelpers::FObjectFinder<UInputAction> LookActionRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Main/Input/InputAction/IA_Look.IA_Look'"));
	if (InputMappingContextRef.Succeeded())
	{
		LookAction = LookActionRef.Object;
	}

	ConstructorHelpers::FObjectFinder<UInputAction> JumpActionRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Main/Input/InputAction/IA_Jump.IA_Jump'"));
	if (InputMappingContextRef.Succeeded())
	{
		JumpAction = JumpActionRef.Object;
	}

	ConstructorHelpers::FClassFinder<UHKAnimInstance> AnimInstanceRef(TEXT("/Script/Engine.Blueprint'/Game/Main/Blueprints/Animation/BP_AnimInstance.BP_AnimInstance_C'"));
	if (AnimInstanceRef.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(AnimInstanceRef.Class);
	}

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

	if (IsValid(ASC))
	{
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &AHKPlayerCharacter::GASInputPressed, 0);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &AHKPlayerCharacter::GASInputReleased, 0);
	}

}

void AHKPlayerCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, InputMoveValue);
	DOREPLIFETIME(ThisClass, InputLookValue);

}

void AHKPlayerCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	AHKPlayerState* GASPS = GetPlayerState<AHKPlayerState>();
	if (GASPS)
	{
		ASC = GASPS->GetAbilitySystemComponent();
		ASC->InitAbilityActorInfo(GASPS, this);
	}

	for (const auto& StartInputAbility : StartInputAbilities)
	{
		FGameplayAbilitySpec StartSpec(StartInputAbility.Value);
		StartSpec.InputID = StartInputAbility.Key;
		ASC->GiveAbility(StartSpec);
	}
}

void AHKPlayerCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState(); 
	AHKPlayerState* GASPS = GetPlayerState<AHKPlayerState>();
	if (GASPS)
	{
		ASC = GASPS->GetAbilitySystemComponent();
		ASC->InitAbilityActorInfo(GASPS, this);

		APlayerController* PlayerController = Cast<APlayerController>(GetController());
		if (PlayerController)
		{
			PlayerController->ConsoleCommand(TEXT("showdebug abilitysystem"));
		}
	}
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

void AHKPlayerCharacter::Look(const FInputActionValue& InValue)
{
	FVector2D LookAxisVector = InValue.Get<FVector2D>();

	AddControllerYawInput(LookAxisVector.X);
	AddControllerPitchInput(LookAxisVector.Y);

	InputLookValue = GetControlRotation();
	UpdateInputLookValue_Server(InputLookValue);
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

