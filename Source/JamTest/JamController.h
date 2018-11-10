// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "JamController.generated.h"

class UUserWidget;
/**
 * 
 */
UCLASS()
class JAMTEST_API AJamController : public APlayerController
{
	GENERATED_BODY()
	
public:

	UFUNCTION(BlueprintCallable, Category = "UI")
	UUserWidget* GetLobbyHUDWidget() const { return LobbyHUDWidget; };

	UFUNCTION(BlueprintCallable, Category = "UI")
	UUserWidget* GetInLobbyMenuWidget() const { return InLobbyMenuWidget; };

	UFUNCTION(BlueprintCallable, Category = "UI")
	void ShowInGameMenu();
	UFUNCTION(BlueprintCallable, Category = "UI")
	void HideInGameMenu();
	UFUNCTION(BlueprintCallable,BlueprintPure, Category = "UI")
	bool IsUIInitialized() const;
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "UI")
	bool IsInGameMenuCollapsed() const;
	UFUNCTION()
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
