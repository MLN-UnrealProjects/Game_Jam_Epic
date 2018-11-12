// Fill out your copyright notice in the Description page of Project Settings.

#include "JamGameInstance.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Runtime/UMG/Public/Blueprint/UserWidget.h"
#include "Runtime/Engine/Classes/GameFramework/GameModeBase.h"
#include "LobbyPlayerController.h"
#include "Runtime/Engine/Public/TimerManager.h"
#include "JamPlayerState.h"

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
	case EGameStatus::Lobby:
		if (InGameStatus != EGameStatus::Playing)
		{
			DestroySession();
		}
		break;
	case EGameStatus::Menu:
		if (MainMenuWidget)
		{
			MainMenuWidget->SetVisibility(ESlateVisibility::Collapsed);
		}
		break;
	case EGameStatus::Startup:
		break;
	case EGameStatus::ServerList:
		if (ServerListWidget)
		{
			ServerListWidget->SetVisibility(ESlateVisibility::Collapsed);
		}
		break;
	case EGameStatus::LoadingScreen:
		if (LoadingWidget)
		{
			LoadingWidget->SetVisibility(ESlateVisibility::Collapsed);
		}
		break;
	case EGameStatus::ErrorDialog:
		if (ErrorDialogWidget)
		{
			ErrorDialogWidget->SetVisibility(ESlateVisibility::Collapsed);
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

void UJamGameInstance::StartPlayingState()
{
	TryChangeStatus(EGameStatus::Playing);

	if (MainMenuWidget)
	{
		MainMenuWidget->RemoveFromViewport();
	}
	if (ServerListWidget)
	{
		ServerListWidget->RemoveFromViewport();
	}
	if (ErrorDialogWidget)
	{
		ErrorDialogWidget->RemoveFromViewport();
	}
	if (LoadingWidget)
	{
		LoadingWidget->RemoveFromViewport();
	}
}
void UJamGameInstance::StartLobbyState()
{
	TryChangeStatus(EGameStatus::Lobby);
	Cast<ALobbyPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0))->SetupLobbyUI();
	if (MainMenuWidget)
	{
		MainMenuWidget->RemoveFromViewport();
	}
	if (ServerListWidget)
	{
		ServerListWidget->RemoveFromViewport();
	}
	if (ErrorDialogWidget)
	{
		ErrorDialogWidget->RemoveFromViewport();
	}
	if (LoadingWidget)
	{
		LoadingWidget->RemoveFromViewport();
	}
}
void UJamGameInstance::CreateNetSession()
{
	ShowLoadingScreen();
	CreateSession();
}

void UJamGameInstance::ShowAndOpenMainMenu(bool OpenLevel)
{
	if (OpenLevel && GameStatus == EGameStatus::Playing || GameStatus == EGameStatus::Lobby)
	{
		UGameplayStatics::OpenLevel(this, MainMenuName);
	}
	MainMenuWidget = ShowWidget(EGameStatus::Menu, MainMenuWidget, MainMenuWidgetClass);
}

void UJamGameInstance::ShowLoadingScreen()
{
	LoadingWidget = ShowWidget(EGameStatus::LoadingScreen, LoadingWidget, LoadingWidgetClass);
}

void UJamGameInstance::ShowServerList()
{
	ServerListWidget = ShowWidget(EGameStatus::ServerList, ServerListWidget, ServerListWidgetClass);
}
void UJamGameInstance::ShowErrorDialog(FText ErrorMsg, bool bDestroySession, float ShowTime)
{
	if (ShowTime < MinErrorShowTime)
	{
		ShowTime = MinErrorShowTime;
	}
	EGameStatus PrevStatus{ GameStatus };

	ErrorDialogWidget = ShowWidget(EGameStatus::ErrorDialog, ErrorDialogWidget, ErrorDialogWidgetClass);

	if (PrevStatus != EGameStatus::ErrorDialog)
	{
		LastErrorMsg = ErrorMsg;
		if (ErrorTimerHandle.IsValid())
		{
			GetTimerManager().ClearTimer(ErrorTimerHandle);
		}
		GetTimerManager().SetTimer(ErrorTimerHandle, this, &UJamGameInstance::CollapseErrorDialog, ShowTime, false);
		if (bDestroySession)
		{
			DestroySession();
		}
	}
}
void UJamGameInstance::LobbyUpdatePlayersMonsterStatus()
{
	PlayersData.SetNum(0);

	for (size_t i = 0; i < GetMaxConnections(); i++)
	{
		ALobbyPlayerController* PC{ Cast<ALobbyPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), i)) };
		if (PC)
		{
			AJamPlayerState* State{ Cast<AJamPlayerState>(PC->PlayerState) };
			if (State)
			{
				if (PC->NetConnection)
				{
					PlayersData.Push(FLobbyPlayerMonsterData{ State->UniqueId, State->GetMonster() });
				}
			}
		}
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
UUserWidget* UJamGameInstance::ShowWidget(EGameStatus InState, UUserWidget* ToInitialize, TSubclassOf<UUserWidget>& Class, bool bForceExec)
{
	if (TryChangeStatus(InState) || bForceExec)
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
			ToInitialize->SetVisibility(ESlateVisibility::Visible);
			if (!ToInitialize->IsInViewport())
			{
				ToInitialize->AddToViewport();
			}
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

		return ToInitialize;
	}
	return ToInitialize;
}

void UJamGameInstance::CollapseErrorDialog()
{
	if (ErrorDialogWidget)
	{
		ErrorDialogWidget->SetVisibility(ESlateVisibility::Collapsed);
	}
	GetTimerManager().ClearTimer(ErrorTimerHandle);

	ShowAndOpenMainMenu();
}
FLobbyPlayerMonsterData::FLobbyPlayerMonsterData(FUniqueNetIdRepl InPlayerConnectionUniqueId, bool IsMonster) : PlayerConnectionUniqueId{ InPlayerConnectionUniqueId }, bMonster{ IsMonster }
{

}