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
				HUDWidget->SetVisibility(ESlateVisibility::Visible);
			}
			InGameMenuWidget = UUserWidget::CreateWidgetOfClass(InGameMenuWidgetClass.Get(), GetGameInstance(), GetWorld(), this); // Create Widget
			if (InGameMenuWidget)
			{
				InGameMenuWidget->AddToViewport(); // Add it to the viewport so the Construct() method in the UUserWidget:: is run.
				InGameMenuWidget->SetVisibility(ESlateVisibility::Collapsed);
			}
		}
	}
}

void AJamController::ShowInGameMenu()
{
	if (InGameMenuWidget)
	{
		if (HUDWidget)
		{
			InGameMenuWidget->SetVisibility(ESlateVisibility::Visible);
			HUDWidget->SetVisibility(ESlateVisibility::Collapsed);

			FInputModeUIOnly Mode;
			Mode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);

			SetInputMode(Mode);
			bShowMouseCursor = true;
		}
	}
}
void AJamController::HideInGameMenu()
{
	if (InGameMenuWidget)
	{
		if (HUDWidget)
		{
			InGameMenuWidget->SetVisibility(ESlateVisibility::Collapsed);
			HUDWidget->SetVisibility(ESlateVisibility::Visible);

			SetInputMode(FInputModeGameOnly{});

			bShowMouseCursor = false;
		}
	}
}

bool AJamController::IsUIInitialized() const
{
	return HUDWidget != nullptr && InGameMenuWidget != nullptr;
}

bool AJamController::IsInGameMenuCollapsed() const
{
	return IsUIInitialized() && InGameMenuWidget->GetVisibility() == ESlateVisibility::Collapsed;
}
