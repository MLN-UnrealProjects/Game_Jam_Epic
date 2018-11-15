// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ServersList.generated.h"


UENUM(BlueprintType)	//"BlueprintType" is essential to include 
enum class EGameStatus : uint8
{
	Playing UMETA(DisplayName = "Playing"),
	Menu UMETA(DisplayName = "Menu"),
	Startup UMETA(DisplayName = "Startup"),
	ServerList UMETA(DisplayName = "Server List"),
	LoadingScreen UMETA(DisplayName = "Loading Screen"),
	ErrorDialog UMETA(DisplayName = "Error Dialog"),
	Lobby UMETA(DisplayName = "Lobby"),
	Unknown UMETA(DisplayName = "Unknown")
};

UENUM(BlueprintType)	//"BlueprintType" is essential to include 
enum class EPlayerType : uint8
{
	Undefined UMETA(DisplayName = "Unknown"),
	Monster UMETA(DisplayName = "Monster"),
	Human UMETA(DisplayName = "Human"),
};

UENUM(BlueprintType)	//"BlueprintType" is essential to include 
enum class ELobbyStatus : uint8
{
	GameAboutToStart UMETA(DisplayName = "Game is about to start"),
	WaitingForMinPlayers UMETA(DisplayName = "Waiting for minimum amount of players for game"),
	WaitingForReadyChecks UMETA(DisplayName = "Waiting for players ready status"),
	Unknown UMETA(DisplayName = "Unknown")
};
UENUM(BlueprintType)	//"BlueprintType" is essential to include 
enum class EMatchStatus : uint8
{
	Unknown UMETA(DisplayName = "Unknown"),
	MatchAboutToStart UMETA(DisplayName = "Match is about to start"),
	MatchOngoing UMETA(DisplayName = "Match is Ongoing"),
	MatchOver UMETA(DisplayName = "Match is Over"),
};
USTRUCT(BlueprintType)
struct JAMTEST_API FLobbyPlayerMonsterData
{
	GENERATED_USTRUCT_BODY()
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerData")
		int32 NetId = -1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerData")
		EPlayerType PlayerCharacter = EPlayerType::Undefined;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerData")
		class APlayerController* PlayerController = nullptr;

	void SetID(int32 NewId) { NetId = NewId; };
	void SetPlayerCharacter(EPlayerType NewPlayerCharacter) { PlayerCharacter = NewPlayerCharacter; };
	void SetPlayerController(APlayerController* NewPlayerController);

	FLobbyPlayerMonsterData() {};
	FLobbyPlayerMonsterData(int32 InPlayerNetId, EPlayerType InPlayerType);
};
USTRUCT(BlueprintType)
struct JAMTEST_API FMatchPlayerModels
{
	GENERATED_USTRUCT_BODY()
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerData")
		class USkeletalMesh* Mesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerData")
		TArray<class UMaterial*> Materials;

		USkeletalMesh* GetMesh() const { return Mesh; };
		TArray<class UMaterial*>& GetMaterials() { return Materials; };

	FMatchPlayerModels() {};
	FMatchPlayerModels(USkeletalMesh* InMesh, TArray<class UMaterial*> InMaterials);
};
/**
 * 
 */
UCLASS()
class JAMTEST_API UServersList : public UUserWidget
{
	GENERATED_BODY()

};
