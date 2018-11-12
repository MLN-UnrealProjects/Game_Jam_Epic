// Fill out your copyright notice in the Description page of Project Settings.

#include "JamCharacter.h"
#include "JamMovementComponent.h"
AJamCharacter::AJamCharacter(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer.SetDefaultSubobjectClass<UJamMovementComponent>(ACharacter::CharacterMovementComponentName))
{
	PrimaryActorTick.bCanEverTick = true;

}
// Sets default values

// Called when the game starts or when spawned
void AJamCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AJamCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AJamCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

