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

    //게임 시작했는데 접속하면 쫒아낸다.
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

            RemainWaitingTimeForPlaying = WaitingTime; // 최소인원이 안된다면 대기 시간 초기화.
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
            // 만약 데디 서버가 게임 세션 서비스들과 연동되어 있다면,
            // 이렇게 레벨을 다시 준비된 뒤 세션 서버한테 알려줌. "새로운 플레이어 들어올 수 있음."
            // 그럼 세션 서비스는 새로운 플레이어들에게 데디 서버의 IP 주소를 전달해줘서 접속 유도.
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


