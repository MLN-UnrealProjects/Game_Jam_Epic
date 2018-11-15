// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Runtime/Engine/Classes/GameFramework/GameMode.h"
#include "ServersList.h" 
#include "JamGameMode.generated.h"

class USkeletalMesh;

/**
 *
 */
UCLASS()
class JAMTEST_API AJamGameMode : public AGameMode
{
	GENERATED_BODY()
		AJamGameMode(const FObjectInitializer& ObjectIn);

protected:
	//virtual UClass* GetDefaultPawnClassForController(AController* InController) override;
	//bool ReadyToStartMatch();
	virtual void Tick(float Deltatime) override;
	virtual void BeginPlay() override;
	EMatchStatus MatchStatus = EMatchStatus::Unknown;
	AActor* GetRandomSpawnLocation();
	UPROPERTY(BlueprintReadOnly)
	TArray<AActor*> SpawnPoints;
public:
	UFUNCTION(BlueprintCallable)
		AActor* PopSpawnPoint();
protected:

	void WaitForPlayersToConnect(class UJamGameInstance * GI);
	void GeneratePlayers(TArray<FLobbyPlayerMonsterData> &Players, class AGamePlayerController * PC);
	void UpdateMatchStatus(class UJamGameInstance * GI);
	UFUNCTION(BlueprintImplementableEvent)
	void SpawnCitizens();

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
		TArray<FMatchPlayerModels> MeshesNPC;
		//TArray<USkeletalMesh*> MeshesNPC;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
		TArray<FMatchPlayerModels> MeshesHumans;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
		TSubclassOf<class AJamCharacter> CitizenPawn;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
		int32 MinCitizenCount = 30;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
		int32 MaxCitizenCount = 100;

	//TArray<USkeletalMesh*> MeshesHumans;
	//UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	//TArray<class UMaterial*> MaterialsNPC;
	//UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	//TArray<class UMaterial*> MaterialsHumans;

	UMaterial* SelectedMaterial;
	TArray<UMaterial*> SelectedMaterials;
	USkeletalMesh* SelectedMesh;
	TSubclassOf<class UAnimInstance> SelectedAnimBP;

public:
	UFUNCTION(BlueprintCallable)
	void SelectModelInfos(TArray<FMatchPlayerModels>& Infos);
	UFUNCTION(BlueprintCallable)
		void SelectModelInfosNPC() { SelectModelInfos(MeshesNPC); };
	UFUNCTION(BlueprintCallable, BlueprintPure)
	USkeletalMesh* GetSelectedMesh() const { return SelectedMesh; };
	UFUNCTION(BlueprintCallable, BlueprintPure)
		TSubclassOf<class UAnimInstance> GetSelectedAnimBP() const { return SelectedAnimBP; };
	UFUNCTION(BlueprintCallable, BlueprintPure)
		UMaterial* GetSelectedMaterial() const { return SelectedMaterial; };
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Match")
	FORCEINLINE EMatchStatus GetMatchStatus() const { return MatchStatus; };

	UFUNCTION(BlueprintCallable, Category = "Setup")
	void PopulateSpawnPoints();

	UFUNCTION(BlueprintImplementableEvent, Category = "Match")
	void MonstersHaveWon();
	UFUNCTION(BlueprintImplementableEvent, Category = "Match")
	void HumansHaveWon();
	UFUNCTION(BlueprintImplementableEvent, Category = "Match")
	void DrawnGame();
};
