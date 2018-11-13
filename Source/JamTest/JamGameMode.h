// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Runtime/Engine/Classes/GameFramework/GameMode.h"
#include "JamGameMode.generated.h"

/**
 * 
 */
UCLASS()
class JAMTEST_API AJamGameMode : public AGameMode
{
	GENERATED_BODY()
	AJamGameMode(const FObjectInitializer& ObjectIn);
	//virtual UClass* GetDefaultPawnClassForController(AController* InController) override;
	//bool ReadyToStartMatch();
	virtual void Tick(float Deltatime) override;
	bool bGameStarted = false;

};
