// Fill out your copyright notice in the Description page of Project Settings.

#include "MonsterMovementComponent.h"

//TODO: implement teleport and (if i have time)dash logic

FNetworkPredictionData_Client* UMonsterMovementComponent::GetPredictionData_Client() const
{
	if (ClientPredictionData == nullptr)
	{
		UMonsterMovementComponent* MutableThis = const_cast<UMonsterMovementComponent*>(this);
		MutableThis->ClientPredictionData = new FNetworkPredictionData_Client_MonsterCharacter(*this);
	}
	return ClientPredictionData;
}


void UMonsterMovementComponent::UpdateFromCompressedFlags(uint8 Flags)
{
	UCharacterMovementComponent::UpdateFromCompressedFlags(Flags);

	bWantsToDash = ((Flags & FSavedMove_Character::FLAG_Custom_0) != 0);
	bTeleport = ((Flags & FSavedMove_Character::FLAG_Custom_1) != 0);
}

FNetworkPredictionData_Client_MonsterCharacter::FNetworkPredictionData_Client_MonsterCharacter(const UMonsterMovementComponent& ClientMovementComponent) : FNetworkPredictionData_Client_Character(ClientMovementComponent)
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
	return Flags;
}

bool FSavedMove_MonsterCharacter::CanCombineWith(const FSavedMovePtr & NewMove, ACharacter * Character, float MaxDelta) const
{
	return FSavedMove_Character::CanCombineWith(NewMove, Character, MaxDelta);
}

void FSavedMove_MonsterCharacter::SetMoveFor(ACharacter * Character, float InDeltaTime, FVector const & NewAccel, FNetworkPredictionData_Client_Character & ClientData)
{
	FSavedMove_Character::SetMoveFor(Character, InDeltaTime, NewAccel, ClientData);
}

void FSavedMove_MonsterCharacter::PrepMoveFor(ACharacter * Character)
{
	FSavedMove_Character::PrepMoveFor(Character);
}

