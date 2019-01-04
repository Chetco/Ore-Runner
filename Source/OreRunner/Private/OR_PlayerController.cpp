// Fill out your copyright notice in the Description page of Project Settings.

#include "OR_PlayerController.h"
#include "OR_pawn.h"

AOR_PlayerController::AOR_PlayerController() {
	// Show the default cursor
	bShowMouseCursor = 1;
	pOR_rot_vector_sum = { 0.0f, 0.0f, 0.0f };
}

void AOR_PlayerController::SetupInputComponent() {
	Super::SetupInputComponent();

	check(InputComponent);

	InputComponent->BindAxis("X_Axis_Mouse", this, &AOR_PlayerController::OR_playerMouseX);
	InputComponent->BindAxis("Y_Axis_Mouse", this, &AOR_PlayerController::OR_playerMouseY);
	InputComponent->BindAxis("X_Axis_Joy", this, &AOR_PlayerController::OR_playerAxisX);
	InputComponent->BindAxis("Y_Axis_Joy", this, &AOR_PlayerController::OR_playerAxisY);
	InputComponent->BindAxis("X_Axis_Key", this, &AOR_PlayerController::OR_playerAxisX);
	InputComponent->BindAxis("Y_Axis_Key", this, &AOR_PlayerController::OR_playerAxisY);
}

void AOR_PlayerController::OR_playerMouseX(const float fAxisValue) {
	float fx = fAxisValue , fy = 0;

	if (GetMousePosition(fx, fy)) {
		int32 center_x, center_y;
		GetViewportSize(center_x, center_y);
		fx = (fx - center_x) / ((float) center_x);
	}

	OR_playerAxisX(fx);
}

void AOR_PlayerController::OR_playerMouseY(const float fAxisValue) {
	float fx = 0, fy = fAxisValue;

	if (GetMousePosition(fx, fy)) {
		int32 center_x, center_y;
		GetViewportSize(center_x, center_y);
		fy = (fy - center_y) / ((float)center_x);
	}

	OR_playerAxisY(fy);
}

void AOR_PlayerController::OR_playerAxisX(const float fAxisValue) {
	AOR_Pawn * ptr = Cast < AOR_Pawn > (GetPawn());
	if (ptr) {
		ptr->OR_pawnXPoint(fAxisValue);
	}
}

void AOR_PlayerController::OR_playerAxisY(const float fAxisValue) {
	AOR_Pawn * ptr = Cast < AOR_Pawn >(GetPawn());
	if (ptr) {
		ptr->OR_pawnYPoint(fAxisValue);
	}
}