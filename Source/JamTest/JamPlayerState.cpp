// Fill out your copyright notice in the Description page of Project Settings.

#include "JamPlayerState.h"
#include "Net/UnrealNetwork.h"
AJamPlayerState::AJamPlayerState()
{
	bReplicates = true;
}
void AJamPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty> & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AJamPlayerState, LobbyStatus);
	DOREPLIFETIME(AJamPlayerState, bReadyCheck);
	DOREPLIFETIME(AJamPlayerState, bMonster);
	DOREPLIFETIME(AJamPlayerState, bWantsToBeMonster);
}

