// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Runtime/Engine/Classes/GameFramework/GameMode.h"
#include "JamGameInstance.h" //TODO: non mi piace quessto include
#include "JamGameMode.generated.h"

UENUM(BlueprintType)	//"BlueprintType" is essential to include 
enum class EMatchStatus : uint8
{
	Unknown UMETA(DisplayName = "Unknown"),
	MatchAboutToStart UMETA(DisplayName = "Match is about to start"),
	MatchOngoing UMETA(DisplayName = "Match is Ongoing"),
	MatchOver UMETA(DisplayName = "Match is Over"),
};
USTRUCT(BlueprintType)
struct JAMTEST_API FMatchPlayerData
{
	GENERATED_USTRUCT_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerData")
	class AGamePlayerController* PC = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerData")
	EPlayerType PlayerType = EPlayerType::Undefined;



	FMatchPlayerData() {};
	FMatchPlayerData(AGamePlayerController* InPC, EPlayerType InPlayerType);
};
/**
 * 
 */
UCLASS()
class JAMTEST_API AJamGameMode : public AGameMode
{
	GENERATED_BODY()
	AJamGameMode(const FObjectInitializer& ObjectIn);
protected:
	//virtual UClass* GetDefaultPawnClassForController(AController* InController) override;
	//bool ReadyToStartMatch();
	virtual void Tick(float Deltatime) override;
	virtual void BeginPlay() override;
	EMatchStatus MatchStatus = EMatchStatus::Unknown;
	AActor* GetRandomSpawnLocation();
	TArray<AActor*> SpawnPoints;
	UPROPERTY(BlueprintReadOnly,Category = "Players")
	TArray<FMatchPlayerData> Players;
	void WaitForPlayersToConnect(class UJamGameInstance * GI);
	void UpdateMatchStatus(class UJamGameInstance * GI);
public:
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Match")
	FORCEINLINE EMatchStatus GetMatchStatus() const { return MatchStatus; };

	UFUNCTION(BlueprintCallable,Category = "Setup")
	void PopulateSpawnPoints();

	UFUNCTION(BlueprintImplementableEvent, Category = "Match")
	void MonstersHaveWon();
	UFUNCTION(BlueprintImplementableEvent, Category = "Match")
	void HumansHaveWon();
	UFUNCTION(BlueprintImplementableEvent, Category = "Match")
	void DrawnGame();
};
