// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "LobbyGameMode.generated.h"

UENUM(BlueprintType)	//"BlueprintType" is essential to include 
enum class ELobbyStatus : uint8
{
	GameAboutToStart UMETA(DisplayName = "Game is about to start"),
	WaitingForMinPlayers UMETA(DisplayName = "Waiting for minimum amount of players for game"),
	WaitingForReadyChecks UMETA(DisplayName = "Waiting for players ready status"),
	Unknown UMETA(DisplayName = "Unknown")
};
/**
 * 
 */
UCLASS()
class JAMTEST_API ALobbyGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ALobbyGameMode();

	UFUNCTION(BlueprintCallable,BlueprintPure,Category = "Network")
	FORCEINLINE int32 GetCurrentlyConnectedPlayersNumber() const { return CurrentlyConnectedPlayers; };
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Network")
	FORCEINLINE ELobbyStatus GetLobbyState() const { return LobbyStatus; };
	UFUNCTION(BlueprintCallable, Category = "Network")
	void SetReadyStatus(class AJamPlayerState* PS, bool bReadyStatus);
	UFUNCTION(BlueprintCallable, Category = "Network")
	void SetWantsToBeMonster(class AJamPlayerState* PS, bool bWantsToBeMonster);
protected:
	UFUNCTION(BlueprintCallable, Category = "Network")
	void StartGame();

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Network")
	FString GameLevelURL = "Game";
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Network")
	float ReadyTimer = 2.5f;

	UFUNCTION()
	void OnLoginLogout(AGameModeBase* GameMode, APlayerController* PC);

	UFUNCTION(NetMulticast,Reliable)
	void SpreadGamePlayerstateInfo();

	virtual void BeginPlay() override;

	virtual void Tick(float Deltatime) override;

	UFUNCTION(BlueprintCallable, Category = "Network")
	void StartCountDownToGame(bool bResetTimer = true);
	UFUNCTION(BlueprintCallable, Category = "Network")
	void AbortCountDownToGame();
private:
	ELobbyStatus LobbyStatus = ELobbyStatus::WaitingForMinPlayers;

	int32 CurrentlyConnectedPlayers = 0;

	FTimerHandle TimerHandle;
};
