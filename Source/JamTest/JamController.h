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
	UUserWidget* GetHUDWidget() const { return HUDWidget; };

	UFUNCTION(BlueprintCallable, Category = "UI")
	UUserWidget* GetInGameMenuWidget() const { return InGameMenuWidget; };

	UFUNCTION(BlueprintCallable, Category = "UI")
	void ShowInGameMenu();
	UFUNCTION(BlueprintCallable, Category = "UI")
	void HideInGameMenu();
	UFUNCTION(BlueprintCallable,BlueprintPure, Category = "UI")
	bool IsUIInitialized() const;
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "UI")
	bool IsInGameMenuCollapsed() const;
protected:
	UPROPERTY(BlueprintReadOnly, Category = "UI")
	UUserWidget* HUDWidget = nullptr;
	UPROPERTY(BlueprintReadOnly, Category = "UI")
	UUserWidget* InGameMenuWidget = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> HUDWidgetClass;
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> InGameMenuWidgetClass;

	UFUNCTION(BlueprintCallable, Category = "UI")
	void SetupUI();
};
