// Fill out your copyright notice in the Description page of Project Settings.

#include "JamGameMode.h"

#include "GamePlayerController.h"
#include "JamCharacter.h"
#include "JamGameInstance.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "Runtime/Engine/Public/EngineUtils.h"
#include "Runtime/Engine/Classes/Engine/TargetPoint.h"
AJamGameMode::AJamGameMode(const FObjectInitializer& ObjectIn) : AGameMode(ObjectIn)
{
	bDelayedStart = true;
	PrimaryActorTick.bCanEverTick = true;
	DefaultPawnClass = nullptr;
	PlayerControllerClass = AGamePlayerController::StaticClass();
}
void AJamGameMode::BeginPlay()
{
	PopulateSpawnPoints();
	MatchStatus = EMatchStatus::MatchAboutToStart;
	Super::BeginPlay();
}
AActor * AJamGameMode::GetRandomSpawnLocation()
{
	return (SpawnPoints.Num() == 0 ? nullptr : SpawnPoints.Pop());
}
void AJamGameMode::PopulateSpawnPoints()
{
	while (SpawnPoints.Num() != 0)
	{
		SpawnPoints.Pop();
	}
	TActorIterator<ATargetPoint> Iterator{ GetWorld() };
	while (Iterator)
	{
		SpawnPoints.Push(*Iterator);
		++Iterator;
	}
}
void AJamGameMode::Tick(float Deltatime)
{
	UJamGameInstance* GI{ Cast<UJamGameInstance>(GetGameInstance()) };

	if (!ensure(GI))
	{
		return;
	}

	switch (MatchStatus)
	{
	case EMatchStatus::Unknown:
		break;
	case EMatchStatus::MatchAboutToStart:
		WaitForPlayersToConnect(GI);
		break;
	case EMatchStatus::MatchOngoing:
		UpdateMatchStatus(GI);
		break;
	case EMatchStatus::MatchOver:
		break;
	default:
		break;
	}
}
void AJamGameMode::UpdateMatchStatus(UJamGameInstance * GI)
{
	bool bAllMonstersDead{ true };
	bool bAllHumansDead{ true };
	for (size_t i = 0; i < Players.Num(); i++)
	{
		FMatchPlayerData Data{ Players[i] };

		switch (Data.PlayerType)
		{
		case EPlayerType::Undefined:
			ensure(false);
			break;
		case EPlayerType::Monster:
			if (Data.PC->IsAlive())
			{
				bAllMonstersDead = false;
			}
			break;
		case EPlayerType::Human:
			if (Data.PC->IsAlive())
			{
				bAllHumansDead = false;
			}
			break;
		default:
			break;
		}

	}

	if (!bAllHumansDead && bAllMonstersDead)
	{
		return;
	}

	if (bAllHumansDead && bAllMonstersDead)
	{
		DrawnGame();
		return;
	}

	if (bAllHumansDead)
	{
		MonstersHaveWon();
	}
	else
	{
		HumansHaveWon();
	}
}
void AJamGameMode::WaitForPlayersToConnect(UJamGameInstance * GI)
{
	while (Players.Num() != 0)
	{
		Players.Pop();
	}

	if (NumPlayers != 0 && NumTravellingPlayers == 0)
	{
		for (size_t i = 0; i < GI->GetMaxConnections(); i++)
		{
			AGamePlayerController* PC{ Cast<AGamePlayerController>(UGameplayStatics::GetPlayerController(GetWorld(),i)) };
			if (PC)
			{

				EPlayerType Type{};
				if (PC->IsMonster())
				{
					Type = EPlayerType::Monster;
				}
				else
				{
					Type = EPlayerType::Human;
				}
				FMatchPlayerData Data{ PC,Type };
				Players.Push(Data);

				TSubclassOf<AJamCharacter> PawnToSpawn{ PC->GetPawnClassToUse() };
				if (PawnToSpawn)
				{
					AActor* SpawnActor{ GetRandomSpawnLocation() };
					APawn* Pawn{ GetWorld()->SpawnActor<APawn>(PawnToSpawn.Get(), SpawnActor->GetTransform()) };
					PC->Possess(Pawn);
				}
			}
		}
		MatchStatus = EMatchStatus::MatchOngoing;
		StartMatch();
	}
}

FMatchPlayerData::FMatchPlayerData(AGamePlayerController * InPC, EPlayerType InPlayerType) : PC{InPC} , PlayerType{ InPlayerType }
{
}
