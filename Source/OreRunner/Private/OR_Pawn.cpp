// Fill out your copyright notice in the Description page of Project Settings.

#include "OR_Pawn.h"
#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/LightComponent.h"
#include "Math/UnrealMathUtility.h"

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

	//Populate Static Mesh Component

	UStaticMesh * heroMesh = LoadObject<UStaticMesh>(nullptr, TEXT("/Game/Collections/fbx/Player.Player")); 
		/* StaticLoadObject is also an option here */
	check(heroMesh != nullptr); // Assert heroMesh is not Null
	OR_pawnStaticMeshComp->SetStaticMesh(heroMesh);

	// Attach static mesh to root component
	OR_pawnStaticMeshComp->SetupAttachment(RootComponent);

	// Allow Simulation by Default
	Cast<UPrimitiveComponent>(OR_pawnStaticMeshComp)->SetSimulatePhysics(true);

	// Create the Pawns Camera Component
	OR_pawnCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));

	//Get a bit of offset for the Camera Component
	OR_pawnCameraComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 750.0f));

	//Rotate it to look down on the player with -y axis and +x axis
	OR_pawnCameraComponent->SetRelativeRotation(FRotator(-90.0f, 0.0f, -90.0f));

	//Attach it to Root Component
	OR_pawnCameraComponent->SetupAttachment(RootComponent);

	//Create Eye Light Component
	pOR_EyeLightComponent->CreateDefaultSubobject<ULightComponent>(TEXT("EyeLight"));

	//Set Relative Location of Eye Light
	pOR_EyeLightComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));

	//Attach it to Root Component
	pOR_EyeLightComponent->SetupAttachment(RootComponent);

	//Default values of integer fields (zeroing out)
	iCosineVal = 0;

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
	PlayerInputComponent->BindAxis("X_Axis_Joy", this, &AOR_Pawn::OR_pawnXPoint);
	PlayerInputComponent->BindAxis("Y_Axis_Joy", this, &AOR_Pawn::OR_pawnYPoint);
	//PlayerInputComponent->BindAxis("X_Axis_Mouse", this, &AOR_Pawn::OR_pawnXPoint); //TODO event returns position of mouse instead
	//PlayerInputComponent->BindAxis("Y_Axis_Mouse", this, &AOR_Pawn::OR_pawnYPoint);
	PlayerInputComponent->BindAxis("X_Axis_Key", this, &AOR_Pawn::OR_pawnXPoint);
	PlayerInputComponent->BindAxis("Y_Axis_Key", this, &AOR_Pawn::OR_pawnYPoint);
	PlayerInputComponent->BindAction("Accelerate", EInputEvent::IE_Pressed, this, &AOR_Pawn::OR_pawnAccelerate);

}


/* THIS IS CALLED FIRST */
UFUNCTION(BlueprintCallable)
void AOR_Pawn::OR_pawnXPoint(const float fAxisValue) {
	if (fAxisValue) {
		//Correct Vertical Displacement and make degrees
		const float fDegrees = FMath::RadiansToDegrees(FMath::FastAsin(fAxisValue));

		//Rotate Root Component Accordingly to ArcCosine
		OR_pawnStaticMeshComp->SetRelativeRotation(FRotator(0.0f, fDegrees, 0.0f), false, nullptr, ETeleportType::None);

		//Set cosine Val (wait is this a bool?)
		iCosineVal = fAxisValue > 0 ? 1 : -1; // I could have rounded instead for same functionality
	}
	else {
		iCosineVal = 0;
	}
}

/* THIS IS CALLED SECOND */
UFUNCTION(BlueprintCallable)
void AOR_Pawn::OR_pawnYPoint(const float fAxisValue) {
	if (fAxisValue) {
		// Flip the Arcsin curve to match the acosine curve with a vertical displacement
		const float fFlipAxisValue = (fAxisValue * -1.0f);

		//Correct Vertical Displacement and make degrees
		const float fDegrees = FMath::RadiansToDegrees(FMath::FastAsin(fFlipAxisValue) + 1.57079633f);

		//Rotate Root Component Accordingly to Arcsine and if ArcCos is positive
		if (iCosineVal > 0) {
			OR_pawnStaticMeshComp->SetRelativeRotation(FRotator(0.0f, fDegrees, 0.0f), false, nullptr, ETeleportType::None);
		}
		else if (fAxisValue < 0) {
			OR_pawnStaticMeshComp->SetRelativeRotation(FRotator(0.0f, -1 * fDegrees, 0.0f), false, nullptr, ETeleportType::None);
		}
	}
}

UFUNCTION(BlueprintCallable)
void AOR_Pawn::OR_pawnAccelerate() {
	// Add a force to the Character
	OR_pawnStaticMeshComp->AddForce(FVector(0.0f, 1.0f, 0.0f), NAME_None, true);
}

