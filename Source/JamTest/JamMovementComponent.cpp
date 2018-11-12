// Fill out your copyright notice in the Description page of Project Settings.

#include "JamMovementComponent.h"
#include "Runtime/Engine/Classes/GameFramework/Pawn.h"
#include "Runtime/Engine/Classes/GameFramework/Character.h"
//TODO: implement teleport and (if i have time)dash logic

FNetworkPredictionData_Client* UJamMovementComponent::GetPredictionData_Client() const
{
	if (ClientPredictionData == nullptr)
	{
		UJamMovementComponent* MutableThis = const_cast<UJamMovementComponent*>(this);
		MutableThis->ClientPredictionData = new FNetworkPredictionData_Client_MonsterCharacter(*this);
	}
	return ClientPredictionData;
}
bool UJamMovementComponent::IsMovingForward() const
{
	if (!PawnOwner)
	{
		return false;
	}
	FVector Forward = PawnOwner->GetActorForwardVector();
	FVector MoveDirection = Velocity.GetSafeNormal();
	//Ignore vertical movement 
	Forward.Z = 0.0f; MoveDirection.Z = 0.0f;
	float VelocityDot = FVector::DotProduct(Forward, MoveDirection);
	return VelocityDot > 0.7f;
	//Check to make sure difference between headings is not too great. 
}

float UJamMovementComponent::GetMaxSpeed() const
{
	float MaxSpeed = Super::GetMaxSpeed();
	if (bWantsToSprint && IsMovingForward())
	{
		MaxSpeed *= SprintSpeedMultiplier;
	}
	return MaxSpeed;
}
float UJamMovementComponent::GetMaxAcceleration() const
{
	float MaxAccel = Super::GetMaxAcceleration();
	if (bWantsToSprint && IsMovingForward())
	{
		MaxAccel *= SprintAccelerationMultiplier;
	}
	return MaxAccel;
}
bool UJamMovementComponent::ServerSetMoveDirection_Validate(const FVector& MoveDir)
{
	return true;
}
void UJamMovementComponent::ServerSetMoveDirection_Implementation(const FVector& MoveDir)
{
	//MoveDirection = DodgeDir;
	MoveDirection = MoveDir;
}
void UJamMovementComponent::OnMovementUpdated(float DeltaSeconds, const FVector& OldLocation, const FVector& OldVelocity)
{
	Super::OnMovementUpdated(DeltaSeconds, OldLocation, OldVelocity);
	if (!CharacterOwner)
	{
		return;
	}
	//Store movement vector 
	if (PawnOwner->IsLocallyControlled())
	{
		MoveDirection = PawnOwner->GetLastMovementInputVector();
	}
	//Send movement vector to server 
	if (PawnOwner->Role < ROLE_Authority)
	{
		ServerSetMoveDirection(MoveDirection);
	}
	//Update dodge movement 
	if (bWantsToDash)
	{
		MoveDirection.Normalize();
		FVector DashVel = MoveDirection * DashStrength;
		//DashVel.Z = 0.0f;
		if (IsMovingOnGround())
		{
			DashVel *= GroundDashStrengthMultiplier;
		}
		else
		{
			DashVel *= AirDashStrengthMultiplier;
		}
		Launch(DashVel);
		bWantsToDash = false;
	}
}
void UJamMovementComponent::UpdateFromCompressedFlags(uint8 Flags)
{
	UCharacterMovementComponent::UpdateFromCompressedFlags(Flags);

	bWantsToDash = ((Flags & FSavedMove_Character::FLAG_Custom_0) != 0);
	bTeleport = ((Flags & FSavedMove_Character::FLAG_Custom_1) != 0);
	bWantsToSprint = ((Flags & FSavedMove_Character::FLAG_Custom_2) != 0);
}

FNetworkPredictionData_Client_MonsterCharacter::FNetworkPredictionData_Client_MonsterCharacter(const UJamMovementComponent& ClientMovementComponent) : FNetworkPredictionData_Client_Character(ClientMovementComponent)
{

}
FSavedMovePtr FNetworkPredictionData_Client_MonsterCharacter::AllocateNewMove()
{
	return FSavedMovePtr(new FSavedMove_MonsterCharacter());
}

void FSavedMove_MonsterCharacter::Clear()
{
	FSavedMove_Character::Clear();
	bWantsToDash = false;
	bTeleport = false;
	bWantsToSprint = false;
	SavedMoveDirection = FVector(0.0f, 0.0f, 0.0f);
}

uint8 FSavedMove_MonsterCharacter::GetCompressedFlags() const
{
	uint8 Flags{ FSavedMove_Character::GetCompressedFlags() };

	if (bWantsToDash)
	{
		Flags |= FLAG_Custom_0;
	}
	if (bTeleport)
	{
		Flags |= FLAG_Custom_1;
	}
	if (bWantsToSprint)
	{
		Flags |= FLAG_Custom_2;
	}
	return Flags;
}

bool FSavedMove_MonsterCharacter::CanCombineWith(const FSavedMovePtr & NewMove, ACharacter * Character, float MaxDelta) const
{
	//This pretty much just tells the engine if it can optimize by combining saved moves. There doesn't appear to be 
	//any problem with leaving it out, but it seems that it's good practice to implement this anyways. 
	if (bWantsToSprint != ((FSavedMove_MonsterCharacter*)&NewMove)->bWantsToSprint)
	{
		return false;
	}
	if (SavedMoveDirection != ((FSavedMove_MonsterCharacter*)&NewMove)->SavedMoveDirection)
	{
		return false;
	}
	return FSavedMove_Character::CanCombineWith(NewMove, Character, MaxDelta);
}

void FSavedMove_MonsterCharacter::SetMoveFor(ACharacter * Character, float InDeltaTime, FVector const & NewAccel, FNetworkPredictionData_Client_Character & ClientData)
{
	FSavedMove_Character::SetMoveFor(Character, InDeltaTime, NewAccel, ClientData);
	
	UJamMovementComponent* CharMov = Cast<UJamMovementComponent>(Character->GetCharacterMovement());
	if (CharMov)
	{
		//This is literally just the exact opposite of UpdateFromCompressed flags. We're taking the input 
		//from the player and storing it in the saved move. 
		bWantsToSprint = CharMov->GetWantsToSprint();
		bWantsToDash = CharMov->GetWantsToDash();
		bTeleport = CharMov->GetTeleport();
		SavedMoveDirection = CharMov->MoveDirection;
	}
}

void FSavedMove_MonsterCharacter::PrepMoveFor(ACharacter * Character)
{
	FSavedMove_Character::PrepMoveFor(Character);
	
	UJamMovementComponent* CharMov = Cast<UJamMovementComponent>(Character->GetCharacterMovement());
	if (CharMov)
	{
		//This is just the exact opposite of SetMoveFor. It copies the state from the saved move to the movement 
		//component before a correction is made to a client. 
		CharMov->MoveDirection = SavedMoveDirection;
		//Don't update flags here. They're automatically setup before corrections using the compressed flag methods.
	}
}

