// Fill out your copyright notice in the Description page of Project Settings.

#include "JamController.h"
#include "Runtime/UMG/Public/Blueprint/UserWidget.h"

void AJamController::SetupUI()
{
	if (IsLocalPlayerController())
	{
		if (HUDWidgetClass && InGameMenuWidgetClass)
		{
			HUDWidget = UUserWidget::CreateWidgetOfClass(HUDWidgetClass.Get(), GetGameInstance(), GetWorld(), this); // Create Widget
			if (HUDWidget)
			{
				HUDWidget->AddToViewport(); // Add it to the viewport so the Construct() method in the UUserWidget:: is run.
			}
			InGameMenuWidget = UUserWidget::CreateWidgetOfClass(InGameMenuWidgetClass.Get(), GetGameInstance(), GetWorld(), this); // Create Widget
		}
	}
}

void AJamController::HideInGameMenu()
{
	if (InGameMenuWidget)
	{
		InGameMenuWidget->RemoveFromParent();
	}
	SetInputMode(FInputModeGameOnly{});
}
