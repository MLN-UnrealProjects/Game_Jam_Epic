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

	UFUNCTION(BlueprintCallable,BlueprintPure,Category = "State")
	ELobbyStatus GetLobbyStatus() const;
	UFUNCTION(BlueprintCallable, Category = "State")
	void SetLobbyStatus(ELobbyStatus InValue);
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "State")
	bool GetReadyCheck() const { return bReadyCheck; };
	UFUNCTION(BlueprintCallable, Category = "State")
	void SetReadyCheck(bool InValue) { bReadyCheck = InValue; };
protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty> & OutLifetimeProps) const override;
private:
	UPROPERTY(Replicated)
	ELobbyStatus LobbyStatus;
	UPROPERTY(Replicated)
	bool bReadyCheck = false;
};
