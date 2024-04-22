// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/HKGameModeBase.h"
#include "Characters/HKCharacterBase.h"
#include "Controllers/HKPlayerControllerBase.h"
#include "Game/HKPlayerState.h"
#include "AbilitySystemComponent.h"
#include "Characters/HKPlayerCharacter.h"
#include "AbilitySystem/AttributeSet/HKCharacterAttributeSet.h"
#include "Game/HKGameState.h"
#include "Kismet/GameplayStatics.h"

void AHKGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	GetWorld()->GetTimerManager().SetTimer(MainTimerHandle, this, &ThisClass::OnMainTimerElapsed, 1.f, true);
	RemainWaitingTimeForPlaying = WaitingTime;
    RemainWaitingTimeForEnding = EndingTime;
}

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

void AHKGameModeBase::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

    AHKGameState* HKGameState = GetGameState<AHKGameState>();
    if (HKGameState == false)
        return;

    //���� �����ߴµ� �����ϸ� �i�Ƴ���.
    if (HKGameState->MatchState != EMatchState::Waiting)
    {
        NewPlayer->SetLifeSpan(0.1f);
        return;
    }


    AHKPlayerState* PlayerState = NewPlayer->GetPlayerState<AHKPlayerState>();
    if (true == ::IsValid(PlayerState))
    {
        UAbilitySystemComponent* ASC = PlayerState->GetAbilitySystemComponent();
        const UHKCharacterAttributeSet* AttributeSet = ASC->GetSet<UHKCharacterAttributeSet>();
        if (AttributeSet)
        {
            AttributeSet->DeadDelegate.AddDynamic(this, &ThisClass::OnControllerDead);
        }
    }

	AHKPlayerControllerBase* LoginPlayerController = Cast<AHKPlayerControllerBase>(NewPlayer);
	if (IsValid(LoginPlayerController))
	{
		AlivePlayerControllers.Add(LoginPlayerController);
        
		LoginPlayerController->NotificationText = FText::FromString(TEXT("Connected to the game server"));
	}

}

void AHKGameModeBase::Logout(AController* Exiting)
{
	Super::Logout(Exiting);

	AHKPlayerControllerBase* ExitingPlayerController = Cast<AHKPlayerControllerBase>(Exiting);
	if (IsValid(ExitingPlayerController) && INDEX_NONE != AlivePlayerControllers.Find(ExitingPlayerController))
	{
		AlivePlayerControllers.Remove(ExitingPlayerController);
		DeadPlayerControllers.Add(ExitingPlayerController);
	}
}

void AHKGameModeBase::OnControllerDead(UAbilitySystemComponent* DeadAbilitySystemComponent)
{
    if (DeadAbilitySystemComponent == nullptr)
        return;

    AHKPlayerCharacter* DeadPlayerCharacter = Cast<AHKPlayerCharacter>(DeadAbilitySystemComponent->GetAvatarActor());
    if (DeadPlayerCharacter)
    {
        AHKPlayerControllerBase* DeadController = Cast<AHKPlayerControllerBase>(DeadPlayerCharacter->GetController());

        if (false == ::IsValid(DeadController) || INDEX_NONE == AlivePlayerControllers.Find(DeadController))
        {
            return;
        }

        DeadController->ShowLooserUI(AlivePlayerControllers.Num());

        AlivePlayerControllers.Remove(DeadController);
        DeadPlayerControllers.Add(DeadController);

    }



}



void AHKGameModeBase::OnMainTimerElapsed()
{
    AHKGameState* HKGameState = GetGameState<AHKGameState>();
    if (HKGameState == false)
        return;

    switch (HKGameState->MatchState)
    {
    case EMatchState::None:
        break;
    case EMatchState::Waiting:
    {
        FString NotificationString = FString::Printf(TEXT(""));

        if (AlivePlayerControllers.Num() < MinimumPlayerCountForPlaying)
        {
            NotificationString = FString::Printf(TEXT("Wait another players for playing."));

            RemainWaitingTimeForPlaying = WaitingTime; // �ּ��ο��� �ȵȴٸ� ��� �ð� �ʱ�ȭ.
        }
        else
        {
            NotificationString = FString::Printf(TEXT("Wait %d seconds for playing."), RemainWaitingTimeForPlaying);

            --RemainWaitingTimeForPlaying;
        }

        if (0 == RemainWaitingTimeForPlaying)
        {
            NotificationString = FString::Printf(TEXT(""));

            HKGameState->MatchState = EMatchState::Playing;
        }

        NotifyToAllPlayer(NotificationString);

        break;
    }
    case EMatchState::Playing:
    {
        /*
        FString NotificationString = FString::Printf(TEXT("%d / %d"), AlivePlayerControllers.Num(), AlivePlayerControllers.Num() + DeadPlayerControllers.Num());

        NotifyToAllPlayer(NotificationString);

        if (AlivePlayerControllers.Num() <= 1)
        {
            MatchState = EMatchState::Ending;
        }
        */

        AHKGameState* SGameState = GetGameState<AHKGameState>();
        if (true == ::IsValid(SGameState))
        {
            SGameState->AlivePlayerControllerCount = AlivePlayerControllers.Num();

            FString NotificationString = FString::Printf(TEXT("%d / %d"), SGameState->AlivePlayerControllerCount, SGameState->AlivePlayerControllerCount + DeadPlayerControllers.Num());

            NotifyToAllPlayer(NotificationString);

            if (SGameState->AlivePlayerControllerCount <= 1)
            {
                AlivePlayerControllers[0]->ShowWinnerUI();
                HKGameState->MatchState = EMatchState::Ending;
            }
        }

        break;
    }
    case EMatchState::Ending:
    {
        FString NotificationString = FString::Printf(TEXT("Waiting %d for returning to lobby."), RemainWaitingTimeForEnding);

        NotifyToAllPlayer(NotificationString);

        --RemainWaitingTimeForEnding;

        if (0 == RemainWaitingTimeForEnding)
        {
            for (auto AliveController : AlivePlayerControllers)
            {
                AliveController->ReturnToLobby();
            }
            for (auto DeadController : DeadPlayerControllers)
            {
                DeadController->ReturnToLobby();
            }

            MainTimerHandle.Invalidate();

            FName CurrentLevelName = FName(UGameplayStatics::GetCurrentLevelName(this));
            UGameplayStatics::OpenLevel(this, CurrentLevelName, true, FString(TEXT("listen")));
            // ���� ���� ������ ���� ���� ���񽺵�� �����Ǿ� �ִٸ�,
            // �̷��� ������ �ٽ� �غ�� �� ���� �������� �˷���. "���ο� �÷��̾� ���� �� ����."
            // �׷� ���� ���񽺴� ���ο� �÷��̾�鿡�� ���� ������ IP �ּҸ� �������༭ ���� ����.
        }
        break;
    }
    case EMatchState::End:
        break;
    default:
        break;
    }
}

void AHKGameModeBase::NotifyToAllPlayer(const FString& NotificationString)
{
    for (auto AlivePlayerController : AlivePlayerControllers)
    {
        if (IsValid(AlivePlayerController))
        {
            AlivePlayerController->NotificationText = FText::FromString(NotificationString);
        }
    }

    for (auto DeadPlayerController : DeadPlayerControllers)
    {
        if (IsValid(DeadPlayerController))
        {
            DeadPlayerController->NotificationText = FText::FromString(NotificationString);
        }
    }
}


