// Fill out your copyright notice in the Description page of Project Settings.

#include "JamGameInstance.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Runtime/UMG/Public/Blueprint/UserWidget.h"
#include "Runtime/Engine/Classes/GameFramework/GameModeBase.h"
bool UJamGameInstance::TryChangeStatus(EGameStatus InGameStatus)
{
	if (GameStatus == InGameStatus)
	{
		return false;
	}

	switch (GameStatus)
	{
	case EGameStatus::Playing:
		DestroySession();
		break;
	case EGameStatus::Menu:
		if (MainMenuWidget)
		{
			MainMenuWidget->SetVisibility(ESlateVisibility::Hidden);
		}
		break;
	case EGameStatus::Startup:
		break;
	case EGameStatus::ServerList:
		if (ServerListWidget)
		{
			ServerListWidget->SetVisibility(ESlateVisibility::Hidden);
		}
		break;
	case EGameStatus::LoadingScreen:
		if (LoadingWidget)
		{
			LoadingWidget->SetVisibility(ESlateVisibility::Hidden);
		}
		break;
	case EGameStatus::ErrorDialog:
		if (ErrorDialogWidget)
		{
			ErrorDialogWidget->SetVisibility(ESlateVisibility::Hidden);
		}
		break;
	case EGameStatus::Unknown:
		break;
	default:
		break;
	}

	GameStatus = InGameStatus;

	return true;
}

void UJamGameInstance::BeginPlayShowMenu()
{
	if (GameStatus == EGameStatus::Startup)
	{
		ShowAndOpenMainMenu();
	}
}

void UJamGameInstance::StartPlayingstate()
{
	TryChangeStatus(EGameStatus::Playing);
}

void UJamGameInstance::CreateNetSession()
{
	ShowLoadingScreen();
	CreateSession();
}

void UJamGameInstance::ShowAndOpenMainMenu()
{
	if (GameStatus == EGameStatus::Playing)
	{
		UGameplayStatics::OpenLevel(this, MainMenuName);
	}
	ShowWidget(EGameStatus::Menu, MainMenuWidget, MainMenuWidgetClass);
}

void UJamGameInstance::ShowLoadingScreen()
{
	ShowWidget(EGameStatus::LoadingScreen, LoadingWidget, LoadingWidgetClass);
}

void UJamGameInstance::ShowServerList()
{
	ShowWidget(EGameStatus::ServerList, ServerListWidget, ServerListWidgetClass);
}
void UJamGameInstance::ShowErrorDialog(FText ErrorMsg)
{
	EGameStatus PrevStatus{ GameStatus };

	ShowWidget(EGameStatus::ErrorDialog, ErrorDialogWidget, ErrorDialogWidgetClass);

	if (PrevStatus != EGameStatus::ErrorDialog)
	{
		LastErrorMsg = ErrorMsg;
		DestroySession();
	}
}
void UJamGameInstance::SetNetworkMode(bool LanModeActive)
{
	if (LanModeActive != bEnableLAN)
	{
		bEnableLAN = LanModeActive;
		NetworkModeChanged();
	}
}
void UJamGameInstance::ShowWidget(EGameStatus InState, UUserWidget * ToInitialize, TSubclassOf<UUserWidget>& Class)
{
	if (TryChangeStatus(InState))
	{
		APlayerController* PC{ UGameplayStatics::GetPlayerController(GetWorld(), 0) };

		if (!ToInitialize)
		{
			if (Class.Get() && PC)
			{
				ToInitialize = UUserWidget::CreateWidgetOfClass(Class.Get(), this, GetWorld(), PC); // Create Widget
			}
		}

		if (ToInitialize)
		{
			ToInitialize->AddToViewport();
		}

		if (PC)
		{
			FInputModeUIOnly Mode{};
			//TODO: set focus to given widget
			//Mode.SetWidgetToFocus(MakeShareable(LoadingWidget)); 
			Mode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
			PC->SetInputMode(Mode);
			PC->bShowMouseCursor = true;
		}
	}
}