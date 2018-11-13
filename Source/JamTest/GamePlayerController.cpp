// Fill out your copyright notice in the Description page of Project Settings.

#include "GamePlayerController.h"
#include "JamGameInstance.h"
TSubclassOf<AJamCharacter> AGamePlayerController::GetPawnClassToUse() const
{
	UJamGameInstance* GI{ Cast<UJamGameInstance>(GetGameInstance()) };

	if (!ensure(GI))
	{
		return nullptr;
	}

	if (GI->GetLocalPlayerType() == EPlayerType::Human)
	{
		return HumanPawn;
	}

	return MonsterPawn;
}
