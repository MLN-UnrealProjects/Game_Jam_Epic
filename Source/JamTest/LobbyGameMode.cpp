// Fill out your copyright notice in the Description page of Project Settings.

#include "LobbyGameMode.h"
#include "Engine/World.h"
#include "Runtime/Engine/Classes/GameFramework/GameStateBase.h"
#include "JamController.h"
#include "JamGameInstance.h"
#include "Runtime/Engine/Public/TimerManager.h"
void ALobbyGameMode::StartGame()
{
	GetWorld()->ServerTravel(GameLevelURL);
}
void ALobbyGameMode::OnLogin(AGameModeBase* GameMode, APlayerController* PC)
{
	if (!HasAuthority())
	{
		return;
	}

	CurrentlyConnectedPlayers = GameState->PlayerArray.Num();
	//AJamController* JamPC{ Cast<AJamController>(PC) };

	//if (JamPC)
	//{
	//	JamPC->SetupLobbyUI();
	//}
	UJamGameInstance* GI{ Cast<UJamGameInstance>(GetGameInstance()) };
	if (GI)
	{
		if (GI->GetMaxConnections() == CurrentlyConnectedPlayers && !bReadyPhase)
		{
			StartCountDownToGame();
		}
	}
}

void ALobbyGameMode::BeginPlay()
{
	FGameModeEvents::GameModePostLoginEvent.AddUFunction(this, FName{ "OnLogin" });
	FGameModeEvents::GameModeLogoutEvent.AddUFunction(this, FName{ "OnLogout" });
	bUseSeamlessTravel = true;
	Super::BeginPlay();
}

void ALobbyGameMode::OnLogout(AGameModeBase * GameMode, AController * PC)
{
	if (!HasAuthority())
	{
		return;
	}

	CurrentlyConnectedPlayers = GameState->PlayerArray.Num();

	UJamGameInstance* GI{ Cast<UJamGameInstance>(GetGameInstance()) };
	if (GI)
	{
		if (GI->GetMaxConnections() != CurrentlyConnectedPlayers && bReadyPhase)
		{
			AbortCountDownToGame();
		}
	}
}

void ALobbyGameMode::StartCountDownToGame()
{
	if (TimerHandle.IsValid())
	{
		AbortCountDownToGame();
	}
	GetWorldTimerManager().SetTimer(TimerHandle, this, &ALobbyGameMode::StartGame, ReadyTimer, false);
	bReadyPhase = true;
}

void ALobbyGameMode::AbortCountDownToGame()
{
	GetWorldTimerManager().ClearTimer(TimerHandle);
	bReadyPhase = false;
}
