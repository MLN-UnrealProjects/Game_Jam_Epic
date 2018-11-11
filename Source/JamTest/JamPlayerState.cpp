// Fill out your copyright notice in the Description page of Project Settings.

#include "JamPlayerState.h"
#include "Net/UnrealNetwork.h"
AJamPlayerState::AJamPlayerState()
{
	bReplicates = true;
}
void AJamPlayerState::SetLobbyStatus(ELobbyStatus InValue)
{
	LobbyStatus = InValue;
}
ELobbyStatus AJamPlayerState::GetLobbyStatus() const
{
	return LobbyStatus;
}
void AJamPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty> & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AJamPlayerState, LobbyStatus);
	DOREPLIFETIME(AJamPlayerState, bReadyCheck);
}

