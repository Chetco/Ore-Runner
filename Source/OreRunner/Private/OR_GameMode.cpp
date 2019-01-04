// Fill out your copyright notice in the Description page of Project Settings.

#include "OR_GameMode.h"
#include "OR_Pawn.h"
#include "OR_PlayerController.h"

AOR_GameMode::AOR_GameMode() {
	DefaultPawnClass = AOR_Pawn::StaticClass();
	PlayerControllerClass = AOR_PlayerController::StaticClass();
}