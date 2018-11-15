// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ServersList.h" //TODO: non mi piace quessto include
#include "GamePlayerController.generated.h"
class UUserWidget;
/**
 * 
 */
UCLASS()
class JAMTEST_API AGamePlayerController : public APlayerController
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category = "Setup",meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class AJamCharacter> MonsterPawn;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Setup", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class AJamCharacter> HumanPawn;

	bool bIsMonster;
public:
	UFUNCTION(BlueprintCallable,BlueprintPure,Category = "Pawn")
	TSubclassOf<class AJamCharacter> GetPawnClassToUse() const;
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Pawn")
	 bool IsMonster() const;
	UFUNCTION(BlueprintCallable, Category = "Pawn")
		 void SetIsMonster(bool val);
	UFUNCTION(BlueprintCallable,BlueprintPure, Category = "Pawn")
	bool IsAlive() const;
	UFUNCTION(Client, Reliable, BlueprintCallable, Category = "EndGame")
		void HumansWon();
	UFUNCTION(Client, Reliable, BlueprintCallable, Category = "EndGame")
		void MonstersWon();
	UFUNCTION(Client,Reliable,BlueprintCallable,Category = "EndGame")
		void DrawnGame();

	UFUNCTION(BlueprintImplementableEvent)
		void ActivateGameoverScreen();

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	UUserWidget* EndGameScreen = nullptr;
	UPROPERTY(BlueprintReadOnly)
	EPlayerType WinningFaction;
};
