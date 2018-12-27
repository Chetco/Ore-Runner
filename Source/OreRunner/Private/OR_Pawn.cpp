// Fill out your copyright notice in the Description page of Project Settings.

#include "OR_Pawn.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
AOR_Pawn::AOR_Pawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Set this pawn to be controlled by the least numbered player by default
	AutoPossessPlayer = EAutoReceiveInput::Player0;

	// Create a default scene component
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

	// Create the physical representation of this 
	OR_pawnStaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));

	// Attach static mesh to root component
	OR_pawnStaticMeshComp->SetupAttachment(RootComponent);

	// Allow Simulation by Default
	Cast<UPrimitiveComponent>(OR_pawnStaticMeshComp)->SetSimulatePhysics(true);

	// Create the Pawns Camera Component
	OR_pawnCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));

	//Get a bit of offset for the Camera Component
	OR_pawnCameraComponent->SetRelativeLocation(FVector(-500.0f, 0.0f, 0.0f));

	//Attach it to Root Component
	OR_pawnCameraComponent->SetupAttachment(RootComponent);

}

// Called when the game starts or when spawned
void AOR_Pawn::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AOR_Pawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AOR_Pawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis("Direction2D", this, &AOR_Pawn::OR_pawnXMov);
	PlayerInputComponent->BindAction("JumpAction", IE_Pressed, this, &AOR_Pawn::OR_pawnJumpMov);
}

UFUNCTION(BlueprintCallable)
void AOR_Pawn::OR_pawnJumpMov() {

	//Let's Get the Mass of the Actor
	float mass = Cast<UPrimitiveComponent>(OR_pawnStaticMeshComp)->GetMass();

	// Let's Bounce this character up high!
	OR_pawnStaticMeshComp->AddImpulse(FVector(0.0, 0.0, 2.0f * mass), NAME_None, true);
}

UFUNCTION(BlueprintCallable)
void AOR_Pawn::OR_pawnXMov(float fAxisValue) {

	//Let's Get the Mass of the Actor
	float mass = Cast<UPrimitiveComponent>(OR_pawnStaticMeshComp)->GetMass();

	//Let's Add Some Accellaration
	OR_pawnStaticMeshComp->AddImpulse(FVector(0.0, fAxisValue * 0.25f * mass, 0.0f), NAME_None, true);
}