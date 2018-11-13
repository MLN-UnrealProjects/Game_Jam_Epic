// Fill out your copyright notice in the Description page of Project Settings.

#include "LobbyPlayerController.h"



#include "Runtime/UMG/Public/Blueprint/UserWidget.h"


void ALobbyPlayerController::SetupLobbyUI()
{
	if (IsLocalPlayerController())
	{
		if (LobbyHUDWidgetClass && InLobbyMenuWidgetClass)
		{
			LobbyHUDWidget = UUserWidget::CreateWidgetOfClass(LobbyHUDWidgetClass.Get(), GetGameInstance(), GetWorld(), this); // Create Widget
			if (LobbyHUDWidget)
			{
				LobbyHUDWidget->AddToViewport(); 
				LobbyHUDWidget->SetVisibility(ESlateVisibility::Visible);
			}
			InLobbyMenuWidget = UUserWidget::CreateWidgetOfClass(InLobbyMenuWidgetClass.Get(), GetGameInstance(), GetWorld(), this); // Create Widget
			if (InLobbyMenuWidget)
			{
				InLobbyMenuWidget->AddToViewport(); 
				InLobbyMenuWidget->SetVisibility(ESlateVisibility::Collapsed);
			}
		}
	}
}

void ALobbyPlayerController::ShowInLobbyMenu()
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
void ALobbyPlayerController::HideInLobbyMenu()
{
	if (InLobbyMenuWidget)
	{
		if (LobbyHUDWidget)
		{
			InLobbyMenuWidget->SetVisibility(ESlateVisibility::Collapsed);
			LobbyHUDWidget->SetVisibility(ESlateVisibility::Visible);

			FInputModeUIOnly Mode;
			Mode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);

			SetInputMode(Mode);
			bShowMouseCursor = true;
		}
	}
}

bool ALobbyPlayerController::IsLobbyUIInitialized() const
{
	return LobbyHUDWidget != nullptr && InLobbyMenuWidget != nullptr;
}

bool ALobbyPlayerController::IsInLobbyMenuCollapsed() const
{
	return IsLobbyUIInitialized() && InLobbyMenuWidget->GetVisibility() == ESlateVisibility::Collapsed;
}

void ALobbyPlayerController::RemoveLobbyWidgets_Implementation()
{
	if (GetLobbyHUDWidget())
	{
		GetLobbyHUDWidget()->SetVisibility(ESlateVisibility::Collapsed);
	}
	if (GetInLobbyMenuWidget())
	{
		GetInLobbyMenuWidget()->SetVisibility(ESlateVisibility::Collapsed);
	}
	FInputModeGameOnly Mode{};
	Mode.SetConsumeCaptureMouseDown(false);
	SetInputMode(Mode);
	bShowMouseCursor = false;
}
