// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "LobbyGameMode.generated.h"

/**
 * 
 */
UCLASS()
class JAMTEST_API ALobbyGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable,BlueprintPure,Category = "Network")
	int32 GetCurrentlyConnectedPlayersNumber() const { return CurrentlyConnectedPlayers; };
protected:
	UFUNCTION(BlueprintCallable, Category = "Network")
	void StartGame();

	UPROPERTY(BlueprintReadWrite, Category = "Network")
	FString GameLevelURL = "Game";
	UPROPERTY(BlueprintReadWrite, Category = "Network")
	float ReadyTimer = 5.0f;

	UFUNCTION()
	void OnLogin(AGameModeBase* GameMode, APlayerController* PC);
	UFUNCTION()
	void OnLogout(AGameModeBase* GameMode, AController* PC);

	virtual void BeginPlay() override;

private:
	int32 CurrentlyConnectedPlayers = 0;
	bool bReadyPhase = false;
	void StartCountDownToGame();
	void AbortCountDownToGame();
	FTimerHandle TimerHandle;
};
