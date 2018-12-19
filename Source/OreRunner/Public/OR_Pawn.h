// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Camera/CameraComponent.h"
#include "OR_Pawn.generated.h"

UCLASS()
class ORERUNNER_API AOR_Pawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AOR_Pawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	// Call to make the Pawn Jump Up a finite amount (Impulse)
	virtual void OR_pawnJumpMov();

	// Call to make the Pawn move @param amount
	virtual void OR_pawnXMov(float);

public:

	// Static Mesh Component
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* OR_pawnStaticMeshComp;
	

	// Camera Viewport Component
	UPROPERTY(EditAnywhere)
	UCameraComponent* OR_pawnCameraComponent;

};
