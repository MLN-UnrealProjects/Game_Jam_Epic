// Fill out your copyright notice in the Description page of Project Settings.

#include "ServersList.h"
#include "Runtime/Engine/Classes/Components/SkeletalMeshComponent.h"
#include "Runtime/Engine/Classes/Materials/Material.h"

FMatchPlayerModels::FMatchPlayerModels(USkeletalMesh* InMesh, TArray<class UMaterial*> InMaterials)
{
	Mesh = InMesh;
	Materials = InMaterials;
}
