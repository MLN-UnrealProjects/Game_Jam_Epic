// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "LobbyPlayerController.generated.h"

class UUserWidget;

/**
 * 
 */
UCLASS()
class JAMTEST_API ALobbyPlayerController : public APlayerController
{
	GENERATED_BODY()
	
	
public:

	UFUNCTION(BlueprintCallable,BlueprintPure, Category = "UI")
		UUserWidget* GetLobbyHUDWidget() const { return LobbyHUDWidget; };

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "UI")
		UUserWidget* GetInLobbyMenuWidget() const { return InLobbyMenuWidget; };

	UFUNCTION(Client, Reliable,BlueprintCallable, Category = "UI")
		void RemoveLobbyWidgets();
	UFUNCTION(BlueprintCallable, Category = "UI")
		void ShowInLobbyMenu();
	UFUNCTION(BlueprintCallable, Category = "UI")
		void HideInLobbyMenu();
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "UI")
		bool IsLobbyUIInitialized() const;
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "UI")
		bool IsInLobbyMenuCollapsed() const;
	UFUNCTION(BlueprintCallable, Category = "Setup")
		void SetupLobbyUI();
protected:
	UPROPERTY(BlueprintReadOnly, Category = "UI")
		UUserWidget* LobbyHUDWidget = nullptr;
	UPROPERTY(BlueprintReadOnly, Category = "UI")
		UUserWidget* InLobbyMenuWidget = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
		TSubclassOf<UUserWidget> LobbyHUDWidgetClass;
	UPROPERTY(EditDefaultsOnly, Category = "UI")
		TSubclassOf<UUserWidget> InLobbyMenuWidgetClass;
	
};
