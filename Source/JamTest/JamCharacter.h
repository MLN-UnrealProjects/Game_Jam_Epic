// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "JamCharacter.generated.h"

UCLASS()
class JAMTEST_API AJamCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AJamCharacter(const FObjectInitializer& ObjectInitializer);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	class USkeletalMeshComponent* JamMeshComponent = nullptr;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(BlueprintReadWrite,EditAnywhere,Category = "Health")
	float Health = 5.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Health")
		float MaxHealth = 10.0f;
	UFUNCTION(NetMulticast, Reliable, BlueprintCallable)
	void SetJamSkelMesh(class USkeletalMesh* InMesh, class UMaterial* InMaterial, TSubclassOf<class UAnimInstance> AnimBP);
	UFUNCTION(NetMulticast, Reliable, BlueprintCallable)
		void SetJamSkelMeshes(class USkeletalMesh* InMesh, const TArray<class UMaterial*>& InMaterials, TSubclassOf<class UAnimInstance> AnimBP);
};
