// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "JamMovementComponent.generated.h"
/**
 *
 */
UCLASS(ClassGroup = (Movement), meta = (BlueprintSpawnableComponent))
class JAMTEST_API UJamMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()

private:
	bool bTeleport = false;
	bool bWantsToSprint = false;
	bool bWantsToDash = false;

	float TeleportCooldownTimer;
public:
	//@return Whether or not the character is currently moving in a forward direction. 
	bool IsMovingForward() const; 

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dash")
	FVector MoveDirection; 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dash")
	float DashStrength = 100.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dash")
	float GroundDashStrengthMultiplier = 1.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dash")
	float AirDashStrengthMultiplier = 1.0f;
	UPROPERTY(EditAnywhere, Category = "Dash") 
	float DashCooldown = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Teleport")
	FTransform TeleportTarget = FTransform::Identity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sprint")
	float SprintSpeedMultiplier = 2.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sprint")
	float SprintAccelerationMultiplier = 2.0f;

	virtual FNetworkPredictionData_Client* GetPredictionData_Client() const override;
	//Sets input flags given compressed flags (Client only)
	virtual void UpdateFromCompressedFlags(uint8 Flags);
	virtual float GetMaxSpeed() const override;
	virtual float GetMaxAcceleration() const override;

	//@brief Event triggered at the end of a movement update 
	virtual void OnMovementUpdated(float DeltaSeconds, const FVector & OldLocation, const FVector & OldVelocity) override; 
	

	UFUNCTION(Unreliable, Server, WithValidation) 
	void ServerSetMoveDirection(const FVector& MoveDir);
	UFUNCTION(BlueprintCallable,BlueprintPure,Category = "Teleport")
	FORCEINLINE bool GetTeleport() const { return bTeleport; };
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Dash")
	FORCEINLINE bool GetWantsToDash() const { return bWantsToDash; };
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Sprint")
	FORCEINLINE bool GetWantsToSprint() const { return bWantsToSprint; };

	UFUNCTION(BlueprintCallable, Category = "Teleport")
	void Teleport(bool InTeleport) { bTeleport = InTeleport; };
	UFUNCTION(BlueprintCallable, Category = "Dash")
	void Dash(bool InWantsToDash) { bWantsToDash = InWantsToDash; };
	UFUNCTION(BlueprintCallable, Category = "Sprint")
	void Sprint(bool InWantsToSprint) { bWantsToSprint = InWantsToSprint; };
};

class JAMTEST_API FNetworkPredictionData_Client_MonsterCharacter : public FNetworkPredictionData_Client_Character
{
public:
	FNetworkPredictionData_Client_MonsterCharacter(const UJamMovementComponent& ClientMovementComponent);
	//@brief Allocates a new copy of our custom saved move 
	virtual FSavedMovePtr AllocateNewMove() override;
};
class JAMTEST_API FSavedMove_MonsterCharacter : public FSavedMove_Character
{
	uint32 bWantsToDash : 1;
	uint32 bWantsToSprint : 1;
	uint32 bTeleport : 1;
	FVector SavedMoveDirection;
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
	//@brief This is used to copy state from the saved move to the character movement component. 
	//This is ONLY used for predictive corrections, the actual data must be sent through RPC. 
	virtual void PrepMoveFor(class ACharacter* Character) override;
};