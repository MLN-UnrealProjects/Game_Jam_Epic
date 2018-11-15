// Fill out your copyright notice in the Description page of Project Settings.

#include "ServersList.h"
#include "Runtime/Engine/Classes/Components/SkeletalMeshComponent.h"
#include "Runtime/Engine/Classes/Materials/Material.h"
#include "Runtime/Engine/Classes/Animation/AnimInstance.h"
FMatchPlayerModels::FMatchPlayerModels(USkeletalMesh* InMesh, TArray<class UMaterial*> InMaterials, TSubclassOf<UAnimInstance> InAnimBP)
{
	Mesh = InMesh;
	Materials = InMaterials;
	AnimBP = InAnimBP;
}
