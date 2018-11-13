// Fill out your copyright notice in the Description page of Project Settings.

#include "JamGameMode.h"

#include "GamePlayerController.h"
#include "JamCharacter.h"
#include "JamGameInstance.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "Runtime/Engine/Public/EngineUtils.h"
#include "Runtime/Engine/Classes/Engine/TargetPoint.h"
AJamGameMode::AJamGameMode(const FObjectInitializer& ObjectIn) : Super(ObjectIn)
{
	bDelayedStart = true;
	PrimaryActorTick.bCanEverTick = true;
	DefaultPawnClass = nullptr;
	PlayerControllerClass = AGamePlayerController::StaticClass();
}
void AJamGameMode::BeginPlay()
{
	Super::BeginPlay();
	PopulateSpawnPoints();
}
AActor * AJamGameMode::GetRandomSpawnLocation()
{
	return (SpawnPoints.Num() == 0 ? nullptr : SpawnPoints.Pop());
}
void AJamGameMode::PopulateSpawnPoints()
{
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

	if (!bGameStarted)
	{
		if (NumPlayers != 0 && NumTravellingPlayers == 0)
		{
			for (size_t i = 0; i < GI->GetMaxConnections(); i++)
			{
				AGamePlayerController* PC{ Cast<AGamePlayerController>(UGameplayStatics::GetPlayerController(GetWorld(),i)) };
				if (PC)
				{
					TSubclassOf<AJamCharacter> PawnToSpawn{ PC->GetPawnClassToUse() };
					if (PawnToSpawn)
					{
						AActor* SpawnActor{ GetRandomSpawnLocation() };
						APawn* Pawn{ GetWorld()->SpawnActor<APawn>(PawnToSpawn.Get(), SpawnActor->GetTransform()) };
						PC->Possess(Pawn);
					}
				}
			}
			bGameStarted = true;
			StartMatch();
			return;
		}
		return;
	}
}
