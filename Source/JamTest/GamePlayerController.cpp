// Fill out your copyright notice in the Description page of Project Settings.

#include "GamePlayerController.h"
#include "JamGameInstance.h"
#include "JamCharacter.h"
#include "Runtime/UMG/Public/Blueprint/UserWidget.h"
TSubclassOf<AJamCharacter> AGamePlayerController::GetPawnClassToUse() const
{
	UJamGameInstance* GI{ Cast<UJamGameInstance>(GetGameInstance()) };

	if (!ensure(GI) || GI->GetLocalPlayerType() == EPlayerType::Undefined)
	{
		return nullptr;
	}

	if (GI->GetLocalPlayerType() == EPlayerType::Human)
	{
		return HumanPawn;
	}

	return MonsterPawn;
}

bool AGamePlayerController::IsMonster() const
{
	return GetPawnClassToUse() == MonsterPawn;
}

bool AGamePlayerController::IsAlive() const
{
	AJamCharacter* Char{ Cast<AJamCharacter>(GetPawn()) };

	if (Char)
	{
		return Char->Health > 0.0f;
	}

	return false;
}


void AGamePlayerController::HumansWon_Implementation()
{
	WinningFaction = EPlayerType::Human;
	ActivateGameoverScreen();
}
void AGamePlayerController::MonstersWon_Implementation()
{
	WinningFaction = EPlayerType::Monster;
	ActivateGameoverScreen();
}
void AGamePlayerController::DrawnGame_Implementation()
{
	WinningFaction = EPlayerType::Undefined;
	ActivateGameoverScreen();
}

