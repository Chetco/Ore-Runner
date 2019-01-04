// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
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
	// Call to point the front of the character based on arcsine of this float value, assumes first call
	virtual void OR_pawnXPoint(const float);

	// Call to point the front of the character based on arccosine of this float value, assumes second call
	virtual void OR_pawnYPoint(const float);

	// Call to add a force to the back of the Character
	virtual void bOR_pawnAccelerateOn();

	// Call to undo force to the back of the Character
	virtual void bOR_pawnAccelerateOff();

public:

	// Static Mesh Component
	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* OR_pawnStaticMeshComp;


	// Camera Viewport Component
	UPROPERTY(EditAnywhere)
	class UCameraComponent* OR_pawnCameraComponent;

	//Light Color of Player Eyes | Used for future game mechanics
	UPROPERTY(EditAnywhere)
	FColor OR_EyeColor;

private:

	//Light Component for Player Eyes
	UPROPERTY(EditAnywhere)
	class UPointLightComponent* pOR_EyeLightComponent;

	int8 pOR_iCosineVal;
	uint8 pOR_flags;
	
private:

	void pOR_addFlag(uint8);
	void pOR_removeFlag(uint8);
};
