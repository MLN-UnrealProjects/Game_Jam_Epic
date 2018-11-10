// Fill out your copyright notice in the Description page of Project Settings.

#include "JamController.h"
#include "Runtime/UMG/Public/Blueprint/UserWidget.h"

//TODO: Solve bug that does not setup ui for pcs


void AJamController::SetupLobbyUI()
{
	if (IsLocalPlayerController()) 
	{
		if (LobbyHUDWidgetClass && InLobbyMenuWidgetClass)
		{
			LobbyHUDWidget = UUserWidget::CreateWidgetOfClass(LobbyHUDWidgetClass.Get(), GetGameInstance(), GetWorld(), this); // Create Widget
			if (LobbyHUDWidget)
			{
				LobbyHUDWidget->AddToViewport(); // Add it to the viewport so the Construct() method in the UUserWidget:: is run.
				LobbyHUDWidget->SetVisibility(ESlateVisibility::Visible);
			}
			InLobbyMenuWidget = UUserWidget::CreateWidgetOfClass(InLobbyMenuWidgetClass.Get(), GetGameInstance(), GetWorld(), this); // Create Widget
			if (InLobbyMenuWidget)
			{
				InLobbyMenuWidget->AddToViewport(); // Add it to the viewport so the Construct() method in the UUserWidget:: is run.
				InLobbyMenuWidget->SetVisibility(ESlateVisibility::Collapsed);
			}
		}
	}
}

void AJamController::ShowInGameMenu()
{
	if (InLobbyMenuWidget)
	{
		if (LobbyHUDWidget)
		{
			InLobbyMenuWidget->SetVisibility(ESlateVisibility::Visible);
			LobbyHUDWidget->SetVisibility(ESlateVisibility::Collapsed);

			FInputModeUIOnly Mode;
			Mode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);

			SetInputMode(Mode);
			bShowMouseCursor = true;
		}
	}
}
void AJamController::HideInGameMenu()
{
	if (InLobbyMenuWidget)
	{
		if (LobbyHUDWidget)
		{
			InLobbyMenuWidget->SetVisibility(ESlateVisibility::Collapsed);
			LobbyHUDWidget->SetVisibility(ESlateVisibility::Visible);

			SetInputMode(FInputModeGameOnly{});

			bShowMouseCursor = false;
		}
	}
}

bool AJamController::IsUIInitialized() const
{
	return LobbyHUDWidget != nullptr && InLobbyMenuWidget != nullptr;
}

bool AJamController::IsInGameMenuCollapsed() const
{
	return IsUIInitialized() && InLobbyMenuWidget->GetVisibility() == ESlateVisibility::Collapsed;
}
