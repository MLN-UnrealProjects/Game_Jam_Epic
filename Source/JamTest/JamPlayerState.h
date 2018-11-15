// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "LobbyGameMode.h" //TODO: This include is not optimal
#include "JamPlayerState.generated.h"
/**
 *
 */
UCLASS()
class JAMTEST_API AJamPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	AJamPlayerState();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Lobby")
	FORCEINLINE ELobbyStatus GetLobbyStatus() const { return LobbyStatus; };
	UFUNCTION(BlueprintCallable, Category = "Lobby")
	void SetLobbyStatus(ELobbyStatus InValue) { LobbyStatus = InValue; };
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Lobby")
	FORCEINLINE bool GetReadyCheck() const { return bReadyCheck; };
	UFUNCTION(BlueprintCallable, Category = "Lobby")
	void SetReadyCheck(bool InValue) { bReadyCheck = InValue; };
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Lobby")
	FORCEINLINE bool GetMonster() const { return bMonster; };
	UFUNCTION(BlueprintCallable, Category = "Lobby")
	void SetMonster(bool InValue) { bMonster = InValue; };
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Lobby")
	FORCEINLINE bool GetWantsToBeMonster() const { return bWantsToBeMonster; };
	UFUNCTION(BlueprintCallable, Category = "Lobby")
	void SetWantsToBeMonster(bool InValue) { bWantsToBeMonster = InValue; };
protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty> & OutLifetimeProps) const override;
private:
	UPROPERTY(Replicated)
	ELobbyStatus LobbyStatus;
	UPROPERTY(Replicated)
	bool bReadyCheck = false;
	UPROPERTY(Replicated)
	bool bMonster = false;
	UPROPERTY(Replicated)
	bool bWantsToBeMonster = false;
};
