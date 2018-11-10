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
		UE_LOG(LogTemp, Warning, TEXT("Current state is unknown"));
		break;
	default:
		UE_LOG(LogTemp, Warning, TEXT("Current state is unknown"));
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
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Current state is not startup"));
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
	UE_LOG(LogTemp, Warning, TEXT("Errod Dialog: %s"), *ErrorMsg.ToString());

	EGameStatus PrevStatus{ GameStatus };

	ShowWidget(EGameStatus::ErrorDialog, ErrorDialogWidget, ErrorDialogWidgetClass);

	if (PrevStatus != EGameStatus::ErrorDialog)
	{
		DestroySession();
	}
}
void UJamGameInstance::ShowWidget(EGameStatus InState, UUserWidget * ToInitialize, TAssetSubclassOf<UUserWidget> Class)
{
	if (TryChangeStatus(InState))
	{
		APlayerController* PC{ GetFirstLocalPlayerController() };
		if (!ensure(PC) || !ensure(GetWorld()) || !ensure(GetWorld()->GetAuthGameMode()))
		{
			return;
		}
		if (!ToInitialize)
		{
			UGameInstance* GI{ (GetWorld()->GetAuthGameMode()->GetGameInstance()) };
			if (ensure(Class) && ensure(GI != NULL))
			{
				ToInitialize = UUserWidget::CreateWidgetOfClass(Class.Get(), GI, GetWorld(), PC); // Create Widget
			}
		}

		if (ToInitialize)
		{
			ToInitialize->AddToViewport();
		}
		FInputModeUIOnly Mode{};
		//TODO: set focus to loading widget
		//Mode.SetWidgetToFocus(MakeShareable(LoadingWidget)); 
		Mode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
		PC->SetInputMode(Mode);
	}
}