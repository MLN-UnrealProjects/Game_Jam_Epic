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
	int32 GetCurrentlyConnectedPlayersNumber() const { return CurrentlyConnectedPlayers; };
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Network")
	ELobbyStatus GetLobbyState() const { return LobbyStatus; };
	UFUNCTION(/*Server, Reliable, WithValidation, */BlueprintCallable, Category = "Network")
	void SetReadyStatus(class AJamPlayerState* PS, bool bReadyStatus);
protected:
	UFUNCTION(BlueprintCallable, Category = "Network")
	void StartGame();
	UFUNCTION(BlueprintImplementableEvent)
	void EndLobbyEnterGame(class APlayerController* PC);

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Network")
	FString GameLevelURL = "Game";
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Network")
	float ReadyTimer = 5.0f;

	//UFUNCTION()
	//void OnLogin(AGameModeBase* GameMode, APlayerController* PC);
	//UFUNCTION()
	//void OnLogout(AGameModeBase* GameMode, AController* PC);

	//virtual void BeginPlay() override;

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
