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
#include "UI/HKWidgetComponent.h"
#include "UI/HKUserWidget.h"
#include "Controllers/HKPlayerControllerBase.h"
#include "Kismet/GameplayStatics.h"
#include "Game/HKGameState.h"


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

	IsZoom = false;

	WeaponSocket_R = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Weapon_R"));
	WeaponSocket_R->SetupAttachment(GetMesh(), TEXT("WeaponSocket_R"));

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

	ConstructorHelpers::FObjectFinder<UInputAction> ReloadActionRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Main/Input/InputAction/IA_Reload.IA_Reload'"));
	if (ReloadActionRef.Succeeded())
	{
		ReloadAction = ReloadActionRef.Object;
	}

	ConstructorHelpers::FClassFinder<UHKAnimInstance> AnimInstanceRef(TEXT("/Script/Engine.Blueprint'/Game/Main/Blueprints/Animation/BP_AnimInstance.BP_AnimInstance_C'"));
	if (AnimInstanceRef.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(AnimInstanceRef.Class);
	}

	HaveWeapons.Init(-1, SlotMaxCount);

	DamageUpWidget = CreateDefaultSubobject<UHKWidgetComponent>(TEXT("DamageUpWidget"));
	DamageUpWidget->SetupAttachment(GetMesh());
	DamageUpWidget->SetRelativeLocation(FVector(0.0f, 0.0f, 220.f));
	static ConstructorHelpers::FClassFinder<UUserWidget> DamageUpWidgetRef(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/Main/UI/WBP_DamageUp.WBP_DamageUp_C'"));
	if (DamageUpWidgetRef.Class)
	{
		DamageUpWidget->SetWidgetClass(DamageUpWidgetRef.Class);
		DamageUpWidget->SetWidgetSpace(EWidgetSpace::Screen);
		DamageUpWidget->SetDrawSize(FVector2D(30.f, 30.f));
		DamageUpWidget->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		DamageUpWidget->SetHiddenInGame(true);
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
	SetupGASInputComponent();
}

void AHKPlayerCharacter::SetupGASInputComponent()
{
	if (IsValid(ASC) && IsValid(InputComponent))
	{
		UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent);
		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Triggered, this, &AHKPlayerCharacter::GASInputPressed, 1);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &AHKPlayerCharacter::GASInputPressed, 2);
		EnhancedInputComponent->BindAction(ReloadAction, ETriggerEvent::Triggered, this, &AHKPlayerCharacter::GASInputPressed, 3);
		EnhancedInputComponent->BindAction(ZoomAction, ETriggerEvent::Started, this, &AHKPlayerCharacter::GASInputPressed, 4);
		EnhancedInputComponent->BindAction(ChangeWeaponAction, ETriggerEvent::Triggered, this, &AHKPlayerCharacter::ChangeWeapon);
	}
}

void AHKPlayerCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, InputMoveValue);
	DOREPLIFETIME(ThisClass, InputLookValue);
	DOREPLIFETIME(ThisClass, EquipWeapon);
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
		
		FGameplayAbilitySpec StartSpec(Cast<AHKWeapon>(Swap->GetDefaultObject())->GetSwapAbility());
		ASC->GiveAbility(StartSpec);
	}

	ASC->OnGameplayEffectAppliedDelegateToSelf.AddUObject(this, &AHKPlayerCharacter::AcquireTag);
	ASC->RegisterGameplayTagEvent(FGameplayTag::RequestGameplayTag(FName("Character.Buff.DamageUp")),
		EGameplayTagEventType::NewOrRemoved).AddUObject(
		this,
		&AHKPlayerCharacter::DamageUpTagChanged
	);
}

void AHKPlayerCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState(); 

	InitAbilityActorInfo();
	SetupGASInputComponent();
	if (GetOwner() == UGameplayStatics::GetPlayerController(this, 0))
	{
		//ASC 초기화가 된 후에 HUD 사용 가능
		AHKPlayerControllerBase* HKControllerBase = Cast<AHKPlayerControllerBase>(GetController());
		HKControllerBase->ShowHUD();
	}

	CompleteSettingASC_Server();

	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if (PlayerController)
	{
		//PlayerController->ConsoleCommand(TEXT("showdebug abilitysystem"));
	}
}

void AHKPlayerCharacter::CompleteSettingASC_Server_Implementation()
{
	for (TSubclassOf<UGameplayEffect> StartEffect : StartEffects)
	{
		FGameplayEffectContextHandle EffectContextHandle = ASC->MakeEffectContext();
		EffectContextHandle.AddSourceObject(this);
		const FGameplayEffectSpecHandle EffectSpecHandle = ASC->MakeOutgoingSpec(StartEffect, 1, EffectContextHandle);
		const FActiveGameplayEffectHandle ActiveEffectHandle = ASC->ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data.Get());
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
	AHKGameState* HKGameState = Cast<AHKGameState>(UGameplayStatics::GetGameState(this));
	if (IsValid(HKGameState) && HKGameState->MatchState != EMatchState::Playing)
	{
		return;
	}

	FGameplayAbilitySpec* Spec = ASC->FindAbilitySpecFromInputID(InputId);
	if (Spec)
	{
		ASC->TryActivateAbility(Spec->Handle);
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

void AHKPlayerCharacter::SetWeapon(AHKWeapon* NewEquipWeapon)
{
	EquipWeapon = NewEquipWeapon;
	WeaponSocket_R->SetSkeletalMesh(&NewEquipWeapon->GetWeaponMesh());
	SetShotMontage(&NewEquipWeapon->GetShotMontage());
	SetReloadMontage(&NewEquipWeapon->GetReloadMontage());
	SetSwapMontage(&NewEquipWeapon->GetSwapMontage());
}

void AHKPlayerCharacter::ChangeWeapon(const FInputActionValue& Value)
{
	int32 InputKey = static_cast<int32>(Value.Get<float>());

	if (InputKey > HaveWeapons.Num())
		return;

	int32 ChangeWeaponKey = HaveWeapons[InputKey - 1];
	if (ChangeWeaponKey == -1)
		return;

	AHKWeapon* NewWeapon = Cast<AHKWeapon>(SwapWeapons.Find(ChangeWeaponKey)->GetDefaultObject());
	if (NewWeapon == nullptr)
		return;

	FGameplayAbilitySpec* Spec = ASC->FindAbilitySpecFromClass(NewWeapon->GetSwapAbility());

	if (Spec)
	{
		ASC->TryActivateAbility(Spec->Handle);
	}
}

void AHKPlayerCharacter::OnRep_ChangeWeapon()
{
	SetWeapon(EquipWeapon);
}

void AHKPlayerCharacter::AcquireTag_Implementation(UAbilitySystemComponent* AbilitySystemComponent, const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle ActiveEffectHandle)
{
	FGameplayTagContainer TagContainer;
	EffectSpec.GetAllAssetTags(TagContainer);
	for (const FGameplayTag& Tag : TagContainer)
	{
		if (Tag.MatchesTag(FGameplayTag::RequestGameplayTag(FName("Character.Event"))))
		{
			// 비어있는 소켓 찾음
			int32 EmptySocketNum;
			if (!HaveWeapons.Find(-1, EmptySocketNum))
				return;

			int32 WeaponNumber = GetWeaponNumber(Tag);
			
			// 이미 있는 경우 총알 충전
			int32 AlreadyWeapon;
			if (HaveWeapons.Find(WeaponNumber, AlreadyWeapon))
			{
				SwapWeapons[WeaponNumber].GetDefaultObject()->AddAmmo();
				return;
			}

			HaveWeapons[EmptySocketNum] = WeaponNumber;
			GetWeaponDelegate.Broadcast(SwapWeapons[WeaponNumber].GetDefaultObject());
		}
		else if (Tag.MatchesTag(FGameplayTag::RequestGameplayTag(FName("Weapon.AddAmmo"))))
		{
			GetWeapon()->AddAmmo();
		}
	}
}

void AHKPlayerCharacter::DamageUpTagChanged_Implementation(const FGameplayTag CallbackTag, int32 NewCount)
{
	bool DamageUp = NewCount > 0;
	DamageUpWidget->SetHiddenInGame(!DamageUp);
}

int32 AHKPlayerCharacter::GetWeaponNumber(const FGameplayTag& Tag)
{
	FWeaponRow* Row = WeaponsDataTable->FindRow<FWeaponRow>(Tag.GetTagName(), TEXT(""));
	if (Row)
	{
		for (auto& SwapWeapon : SwapWeapons)
		{
			if (Row->WeaponClass == SwapWeapon.Value)
			{
				return SwapWeapon.Key;
			}
		}
	}
	return -1;
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

