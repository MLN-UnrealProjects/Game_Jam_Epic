// Fill out your copyright notice in the Description page of Project Settings.

#include "LobbyGameMode.h"
#include "Engine/World.h"
#include "Runtime/Engine/Classes/GameFramework/GameStateBase.h"
#include "JamGameInstance.h"
#include "Runtime/Engine/Public/TimerManager.h"
#include "JamPlayerState.h"
#include "UnrealNetwork.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "LobbyPlayerController.h"
ALobbyGameMode::ALobbyGameMode()
{
	bReplicates = true;
	bUseSeamlessTravel = true;
	PrimaryActorTick.bCanEverTick = true;
}
void ALobbyGameMode::StartGame()
{
	UJamGameInstance* GI{ Cast<UJamGameInstance>(GetGameInstance()) };

	if (!ensure(GI))
	{
		return;
	}

	GI->LobbyUpdatePlayersMonsterStatus();

	for (size_t i = 0; i < GI->GetMaxConnections(); i++)
	{
		ALobbyPlayerController* PC{ Cast<ALobbyPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), i)) };
		if (PC)
		{
			PC->RemoveLobbyWidgets();
		}
	}

	GetWorld()->ServerTravel(GameLevelURL);
}
void ALobbyGameMode::OnLoginLogout(AGameModeBase * GameMode, APlayerController * PC)
{
	Tick(0.0f);
}
void ALobbyGameMode::BeginPlay()
{
	Super::BeginPlay();
	FGameModeEvents::GameModePostLoginEvent.AddUFunction(this, FName{ "OnLoginLogout" });
	FGameModeEvents::GameModeLogoutEvent.AddUFunction(this, FName{ "OnLoginLogout" });
}
void ALobbyGameMode::Tick(float Deltatime)
{
	//resets lobby status, to be filled later
	LobbyStatus = ELobbyStatus::Unknown;

	TArray<APlayerState*> Players{ GameState->PlayerArray };

	CurrentlyConnectedPlayers = Players.Num();

	UJamGameInstance* GI{ Cast<UJamGameInstance>(GetGameInstance()) };

	if (!ensure(GI))
	{
		return;
	}

	//if values are valid initialize lobby status
	LobbyStatus = ELobbyStatus::WaitingForMinPlayers;
	if (GI->GetMinConnections() <= CurrentlyConnectedPlayers)
	{
		LobbyStatus = ELobbyStatus::WaitingForReadyChecks;
	}

	//if we are waiting for players ready status check all players current ready check status to verify if they are all ready
	if (LobbyStatus == ELobbyStatus::WaitingForReadyChecks)
	{
		bool bAllPlayersReady{ true };
		for (size_t i = 0; i < Players.Num(); i++)
		{
			AJamPlayerState* PS{ Cast<AJamPlayerState>(Players[i]) };
			if (PS)
			{
				if (!PS->GetReadyCheck())
				{
					bAllPlayersReady = false;
					break;
				}
			}
		}

		if (bAllPlayersReady)
		{
			LobbyStatus = ELobbyStatus::GameAboutToStart;
		}
	}

	//Update all players about the lobby status and gets ready for monster/human selection logic

	TArray<APlayerState*> WantToBeMonsters;
	TArray<APlayerState*> WantToBeHumans;
	for (size_t i = 0; i < Players.Num(); i++)
	{
		AJamPlayerState* PS{ Cast<AJamPlayerState>(Players[i]) };
		if (PS)
		{
			PS->SetLobbyStatus(LobbyStatus);
			if (PS->GetWantsToBeMonster())
			{
				WantToBeMonsters.Push(PS);
			}
			else
			{
				WantToBeHumans.Push(PS);
			}
		}
	}

	//Monster/human selection logic + all players update

	uint32 HumansCounter{ 0 };
	uint32 MonstersCounter{ 0 };
	bool bNextIsMonster{ true };

	while (WantToBeHumans.Num() != 0 || WantToBeMonsters.Num() != 0)
	{
		if (bNextIsMonster)
		{
			AJamPlayerState* PS{(WantToBeMonsters.Num() != 0 ? Cast<AJamPlayerState>(WantToBeMonsters.Pop()) : Cast<AJamPlayerState>(WantToBeHumans.Pop()))};

			if (PS)
			{
				PS->SetMonster(true);
			}

			MonstersCounter++;
		}
		else
		{
			AJamPlayerState* PS{ (WantToBeHumans.Num() != 0 ? Cast<AJamPlayerState>(WantToBeHumans.Pop()) : Cast<AJamPlayerState>(WantToBeMonsters.Pop())) };
			
			if (PS)
			{
				PS->SetMonster(false);
			}

			HumansCounter++;
		}

		bNextIsMonster = (static_cast<float>(HumansCounter) / MonstersCounter > GI->GetRateoHumansPerMonstersInGame());
	}

	GI->LobbyUpdatePlayersMonsterStatus();

	//if game is about to begin starts count down to start game, otherwise block countdown
	switch (LobbyStatus)
	{
	case ELobbyStatus::GameAboutToStart:
		StartCountDownToGame(false);
		break;
	case ELobbyStatus::WaitingForMinPlayers:
	case ELobbyStatus::WaitingForReadyChecks:
	case ELobbyStatus::Unknown:
	default:
		AbortCountDownToGame();
		break;
	}
}

void ALobbyGameMode::StartCountDownToGame(bool bResetTimer)
{
	if (TimerHandle.IsValid())
	{
		if (!bResetTimer)
		{
			return;
		}
		AbortCountDownToGame();
	}
	GetWorldTimerManager().SetTimer(TimerHandle, this, &ALobbyGameMode::StartGame, ReadyTimer, false);
}

void ALobbyGameMode::AbortCountDownToGame()
{
	GetWorldTimerManager().ClearTimer(TimerHandle);
}

void ALobbyGameMode::SetReadyStatus(AJamPlayerState * PS, bool bReadyStatus)
{
	if (PS)
	{
		PS->SetReadyCheck(bReadyStatus);
	}
}

void ALobbyGameMode::SetWantsToBeMonster(AJamPlayerState * PS, bool bWantsToBeMonster)
{
	if (PS)
	{
		PS->SetWantsToBeMonster(bWantsToBeMonster);
	}
}
