// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "MonsterMovementComponent.generated.h"
/**
 *
 */
UCLASS(ClassGroup = (Movement), meta = (BlueprintSpawnableComponent))
class JAMTEST_API UMonsterMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()

private:
	bool bTeleport = false;
	bool bWantsToDash = false;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category = "Dash")
	float DashMaxMovementAmount = 500.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dash")
	float DashMovementSpeed = 100.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dash")
	bool bUnobstructedDash = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Teleport")
	FTransform TeleportTarget = FTransform::Identity;

	virtual FNetworkPredictionData_Client* GetPredictionData_Client() const override;
	//Sets input flags given compressed flags (Client only)
	virtual void UpdateFromCompressedFlags(uint8 Flags);

	UFUNCTION(BlueprintCallable,BlueprintPure,Category = "Teleport")
	FORCEINLINE bool GetTeleport() const { return bTeleport; };
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Dash")
	FORCEINLINE bool GetWantsToDash() const { return bWantsToDash; };

	UFUNCTION(BlueprintCallable, Category = "Teleport")
	void Teleport(bool InTeleport) { bTeleport = InTeleport; };
	UFUNCTION(BlueprintCallable, Category = "Dash")
	void Dash(bool InWantsToDash) { bWantsToDash = InWantsToDash; };
};

class JAMTEST_API FNetworkPredictionData_Client_MonsterCharacter : public FNetworkPredictionData_Client_Character
{
public:
	FNetworkPredictionData_Client_MonsterCharacter(const UMonsterMovementComponent& ClientMovementComponent);
	//@brief Allocates a new copy of our custom saved move 
	virtual FSavedMovePtr AllocateNewMove() override;
};
class JAMTEST_API FSavedMove_MonsterCharacter : public FSavedMove_Character
{
	uint32 bWantsToDash : 1;
	uint32 bTeleport : 1;
	//@brief Resets all saved variables.
	virtual void Clear() override;
	//@brief Store input commands in the compressed flags
	virtual uint8 GetCompressedFlags() const;
	//@brief This is used to check whether or not two moves can be combined into one. 
	//Basically you just check to make sure that the saved variables are the same
	virtual bool CanCombineWith(const FSavedMovePtr& NewMove, ACharacter* Character, float MaxDelta) const override;
	//@brief Sets up the move before sending it to the server
	virtual void SetMoveFor(ACharacter* Character, float InDeltaTime, FVector const& NewAccel, class FNetworkPredictionData_Client_Character & ClientData) override;
	//@brief Sets variables on character movement component before making a predictive correction.
	virtual void PrepMoveFor(class ACharacter* Character) override;
};