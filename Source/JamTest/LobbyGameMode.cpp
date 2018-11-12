// Fill out your copyright notice in the Description page of Project Settings.

#include "LobbyGameMode.h"
#include "Engine/World.h"
#include "Runtime/Engine/Classes/GameFramework/GameStateBase.h"
#include "JamGameInstance.h"
#include "Runtime/Engine/Public/TimerManager.h"
#include "JamPlayerState.h"
#include "UnrealNetwork.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
ALobbyGameMode::ALobbyGameMode()
{
	bReplicates = true;
	bUseSeamlessTravel = true;
	PrimaryActorTick.bCanEverTick = true;
}
void ALobbyGameMode::StartGame()
{
	for (size_t i = 0; i < GameState->PlayerArray.Num(); i++)
	{

		auto PC = UGameplayStatics::GetPlayerController(GetWorld(), i);
		if (PC)
		{
			EndLobbyEnterGame(PC);
		}
	}
	GetWorld()->ServerTravel(GameLevelURL);
}
void ALobbyGameMode::Tick(float Deltatime)
{
	LobbyStatus = ELobbyStatus::Unknown;

	TArray<APlayerState*> Players{ GameState->PlayerArray };

	CurrentlyConnectedPlayers = Players.Num();

	UJamGameInstance* GI{ Cast<UJamGameInstance>(GetGameInstance()) };

	if (!ensure(GI))
	{
		return;
	}

	LobbyStatus = ELobbyStatus::WaitingForMinPlayers;
	if (GI->GetMinConnections() <= CurrentlyConnectedPlayers)
	{
		LobbyStatus = ELobbyStatus::WaitingForReadyChecks;
	}

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

	for (size_t i = 0; i < Players.Num(); i++)
	{
		AJamPlayerState* PS{ Cast<AJamPlayerState>(Players[i]) };
		if (PS)
		{
			PS->SetLobbyStatus(LobbyStatus);
		}
	}

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
	PS->SetReadyCheck(bReadyStatus);
}
