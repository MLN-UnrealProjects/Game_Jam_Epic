// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GamePlayerController.generated.h"

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

public:
	UFUNCTION(BlueprintCallable,BlueprintPure,Category = "Pawn")
	TSubclassOf<class AJamCharacter> GetPawnClassToUse() const;
};
