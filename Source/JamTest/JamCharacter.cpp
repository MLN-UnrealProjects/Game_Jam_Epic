// Fill out your copyright notice in the Description page of Project Settings.

#include "JamCharacter.h"
#include "JamMovementComponent.h"
#include "Runtime/Engine/Classes/Engine/SkeletalMesh.h"
#include "Runtime/Engine/Classes/Components/SkeletalMeshComponent.h"
#include "Runtime/Engine/Classes/Materials/Material.h"
#include "Runtime/Engine/Classes/Components/CapsuleComponent.h"
AJamCharacter::AJamCharacter(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer/*.DoNotCreateDefaultSubobject(MeshComponentName)*/.SetDefaultSubobjectClass<UJamMovementComponent>(ACharacter::CharacterMovementComponentName))
{
	PrimaryActorTick.bCanEverTick = true;

	//JamMeshComponent = CreateOptionalDefaultSubobject<USkeletalMeshComponent>("JamSkelMeshComp");
	//if (JamMeshComponent)
	//{
	//	JamMeshComponent->AlwaysLoadOnClient = true;
	//	JamMeshComponent->AlwaysLoadOnServer = true;
	//	JamMeshComponent->bOwnerNoSee = false;
	//	JamMeshComponent->MeshComponentUpdateFlag = EMeshComponentUpdateFlag::AlwaysTickPose;
	//	JamMeshComponent->bCastDynamicShadow = true;
	//	JamMeshComponent->bAffectDynamicIndirectLighting = true;
	//	JamMeshComponent->PrimaryComponentTick.TickGroup = TG_PrePhysics;
	//	JamMeshComponent->SetupAttachment(GetCapsuleComponent());
	//	JamMeshComponent->RelativeRotation = FRotator{ 0.0f,-90.0f,0.0f };
	//	JamMeshComponent->SetRelativeLocation(FVector{ 0.0f,0.0f,-GetCapsuleComponent()->GetScaledCapsuleHalfHeight() });
	//	static FName MeshCollisionProfileName(TEXT("CharacterMesh"));
	//	JamMeshComponent->SetCollisionProfileName(MeshCollisionProfileName);
	//	JamMeshComponent->bGenerateOverlapEvents = false;
	//	JamMeshComponent->SetCanEverAffectNavigation(false);
	//}

	JamMeshComponent = GetMesh();
	JamMeshComponent->RelativeRotation = FRotator{ 0.0f,-90.0f,0.0f };
	JamMeshComponent->SetRelativeLocation(FVector{ 0.0f,0.0f,-GetCapsuleComponent()->GetScaledCapsuleHalfHeight() });
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

void AJamCharacter::SetJamSkelMesh_Implementation(USkeletalMesh* InMesh, UMaterial* InMaterial)
{
	if (JamMeshComponent)
	{
		JamMeshComponent->SetSkeletalMesh(InMesh);
		JamMeshComponent->SetMaterial(0, InMaterial);
		UE_LOG(LogTemp, Warning, TEXT("Meshes: %s spawned with %s and %s"), *GetName(), (InMesh ? *InMesh->GetName() : *FString{ "No Mesh" }), (InMaterial ? *InMaterial->GetName() : *FString{ "No Material" }))
	}
}