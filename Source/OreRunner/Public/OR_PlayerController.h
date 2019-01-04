// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "OR_PlayerController.generated.h"

/**
 * OR_PlayerController
 * Class controlling the standard input for the Player in Ore-Runner
 */
UCLASS()
class ORERUNNER_API AOR_PlayerController : public APlayerController
{
	GENERATED_BODY()

// Input Component and obligatory components
public:

	//Constructor
	AOR_PlayerController();

	// Setup Player Controller Components
	virtual void SetupInputComponent() override;

// Callback Functions
public:
	//Call back functions to clamp mouse value to center of viewport
	virtual void OR_playerMouseX(const float);
	virtual void OR_playerMouseY(const float);

	//Call back functions for pointing pawn Also called by above functions typically
	virtual void OR_playerAxisX(const float);
	virtual void OR_playerAxisY(const float);

private:
	UPROPERTY(EditAnywhere)
	FVector pOR_rot_vector_sum;
};