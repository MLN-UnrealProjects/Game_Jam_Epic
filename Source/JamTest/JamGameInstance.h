// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "JamGameInstance.generated.h"

UENUM(BlueprintType)	//"BlueprintType" is essential to include 
enum class EGameStatus : uint8
{
	Playing UMETA(DisplayName = "Playing"),
	Menu UMETA(DisplayName = "Menu"),
	Startup UMETA(DisplayName = "Startup"),
	ServerList UMETA(DisplayName = "Server List"),
	LoadingScreen UMETA(DisplayName = "Loading Screen"),
	ErrorDialog UMETA(DisplayName = "Error Dialog"),
	Unknown UMETA(DisplayName = "Unknown")
};

class UUserWidget;
/**
 * 
 */
UCLASS()
class JAMTEST_API UJamGameInstance : public UGameInstance
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category = "Game")
	EGameStatus GameStatus = EGameStatus::Startup;

	UPROPERTY(EditDefaultsOnly,Category = "Setup")
	FName MainMenuName = "MainMenu";

	/**
	 * Tries to change current game status to the given value. If game status is same as current status returns false, otherwise true
	 */
	UFUNCTION(BlueprintCallable, Category = "Game")
	bool TryChangeStatus(EGameStatus InGameStatus);

	UFUNCTION(BlueprintImplementableEvent) //esposto come evento in bp. Alternativa che permette di implementare roba anche in c++: BlueprintNativeEvent
	void DestroySession();
	UFUNCTION(BlueprintImplementableEvent) //esposto come evento in bp. Alternativa che permette di implementare roba anche in c++: BlueprintNativeEvent
	void CreateSession();

	UPROPERTY()
	UUserWidget* MainMenuWidget;
	UPROPERTY()
	UUserWidget* ServerListWidget;
	UPROPERTY()
	UUserWidget* LoadingWidget;
	UPROPERTY()
	UUserWidget* ErrorDialogWidget;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Setup")
	bool bEnableLAN = true;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Setup")
	int32 MaxConnections = 4;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> LoadingWidgetClass;
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> MainMenuWidgetClass;
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> ErrorDialogWidgetClass;
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> ServerListWidgetClass;
public:
	UFUNCTION(BlueprintCallable, Category  = "Setup")
	void BeginPlayShowMenu();
	UFUNCTION(BlueprintCallable, Category = "Game")
	void StartPlayingstate();
	UFUNCTION(BlueprintCallable, Category = "Game")
	void CreateNetSession();
	UFUNCTION(BlueprintCallable, Category = "UI")
	void ShowAndOpenMainMenu();
	UFUNCTION(BlueprintCallable, Category = "UI")
	void ShowLoadingScreen();
	UFUNCTION(BlueprintCallable, Category = "UI")
	void ShowServerList();
	UFUNCTION(BlueprintCallable, Category = "UI")
	void ShowErrorDialog(FText ErrorMsg);
private:
	void ShowWidget(EGameStatus InState, UUserWidget* ToInitialize, TSubclassOf<UUserWidget>& Class);
};
