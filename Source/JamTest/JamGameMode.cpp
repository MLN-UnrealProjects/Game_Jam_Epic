// Fill out your copyright notice in the Description page of Project Settings.

#include "JamGameMode.h"

#include "GamePlayerController.h"

AJamGameMode::AJamGameMode(const FObjectInitializer& ObjectIn) : Super(ObjectIn)
{
	bDelayedStart = true;
	PrimaryActorTick.bCanEverTick = true;
	DefaultPawnClass = nullptr;
	PlayerControllerClass = AGamePlayerController::StaticClass();
}

void AJamGameMode::Tick(float Deltatime)
{
	if (!bGameStarted)
	{
		if (NumTravellingPlayers == 0)
		{
			bGameStarted = true;
			StartMatch();
		}
		return;
	}
}
