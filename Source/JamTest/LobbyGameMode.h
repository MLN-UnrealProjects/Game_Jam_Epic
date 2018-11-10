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
	
	
protected:
	UFUNCTION(BlueprintCallable, Category = "Network")
	void StartGame();

	UPROPERTY(BlueprintReadWrite, Category = "Network")
	FString GameLevelURL = "Game";

	UFUNCTION()
	void PostLogin(AGameModeBase* GameMode, APlayerController* PC);
};
