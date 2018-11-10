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

	UFUNCTION(BlueprintCallable)
	UUserWidget* GetHUDWidget() const { return HUDWidget; };

	UFUNCTION(BlueprintCallable)
	UUserWidget* GetInGameMenuWidget() const { return InGameMenuWidget; };

	UFUNCTION(BlueprintCallable)
	void HideInGameMenu();
protected:
	UPROPERTY(BlueprintReadOnly, Category = "UI")
	UUserWidget* HUDWidget = nullptr;
	UPROPERTY(BlueprintReadOnly, Category = "UI")
	UUserWidget* InGameMenuWidget = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TAssetSubclassOf<UUserWidget> HUDWidgetClass;
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TAssetSubclassOf<UUserWidget> InGameMenuWidgetClass;

	UFUNCTION(BlueprintCallable)
	void SetupUI();
};
