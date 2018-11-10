// Fill out your copyright notice in the Description page of Project Settings.

#include "LobbyGameMode.h"
#include "Engine/World.h"


void ALobbyGameMode::StartGame()
{
	FGameModeEvents::GameModePostLoginEvent.AddUFunction(this, FName{"ALobbyGameMode::PostLogin"});
	GetWorld()->ServerTravel(GameLevelURL);
}
void ALobbyGameMode::PostLogin(AGameModeBase* GameMode, APlayerController* PC)
{

}