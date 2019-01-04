// Fill out your copyright notice in the Description page of Project Settings.

#include "OR_Pawn.h"
#include "Components/SceneComponent.h"
#include "Math/UnrealMathUtility.h"
#include "Camera/CameraComponent.h"
#include "Components/PointLightComponent.h"
#include "Components/StaticMeshComponent.h"

enum AOR_PawnFlags:uint8 {
	ACCELERATE_PAWN = 1
};

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

	//Attach it to Static Mesh Component
	OR_pawnCameraComponent->SetupAttachment(OR_pawnStaticMeshComp);

	//However we don't want it to rotate with the static Mesh
	OR_pawnCameraComponent->bAbsoluteRotation = 1;

	//Create Eye Light Component
	pOR_EyeLightComponent=CreateDefaultSubobject<UPointLightComponent>(TEXT("EyeLight"));

	//Set Relative Location of Eye Light
	pOR_EyeLightComponent->SetRelativeLocation(FVector(57.0f, -65.0f, 80.0f));
	
	//Set Eye Light to a comfortable intensity
	pOR_EyeLightComponent->SetIntensity(1000.0f);

	//Set Eye Color and respective color
	pOR_EyeLightComponent->SetLightColor(FColor(255, 0, 0, 255));
	OR_EyeColor = FColor(255, 0, 0, 255);

	//Set Attenuation Radius to something small
	pOR_EyeLightComponent->SetAttenuationRadius(30.0f);

	//Attach it to Static Mesh Component NOT Root Component
	pOR_EyeLightComponent->SetupAttachment(OR_pawnStaticMeshComp);

	//Default values of integer fields (zeroing out)
	pOR_iCosineVal = 0;

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

	if (pOR_flags & AOR_PawnFlags::ACCELERATE_PAWN) {
		// Add a force to the Character
		OR_pawnStaticMeshComp->AddForce(FVector(0.0f, -10 * OR_pawnStaticMeshComp->GetMass(), 0.0f), NAME_None, true);
	}

}

// Called to bind functionality to input
void AOR_Pawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAction("Accelerate", EInputEvent::IE_Pressed, this, &AOR_Pawn::bOR_pawnAccelerateOn);
	PlayerInputComponent->BindAction("Accelerate", EInputEvent::IE_Released, this, &AOR_Pawn::bOR_pawnAccelerateOff);

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
		pOR_iCosineVal = fAxisValue > 0 ? 1 : -1; // I could have ceilinged instead for same functionality
	}
	else {
		pOR_iCosineVal = 0;
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
		if (pOR_iCosineVal >= 0) {
			OR_pawnStaticMeshComp->SetRelativeRotation(FRotator(0.0f, fDegrees, 0.0f), false, nullptr, ETeleportType::None);
		}
		else if (fAxisValue < 0) {
			OR_pawnStaticMeshComp->SetRelativeRotation(FRotator(0.0f, -1 * fDegrees, 0.0f), false, nullptr, ETeleportType::None);
		}
	}
}

UFUNCTION(BlueprintCallable)
void AOR_Pawn::bOR_pawnAccelerateOn() {
	pOR_addFlag(AOR_PawnFlags::ACCELERATE_PAWN);
	
}

UFUNCTION(BlueprintCallable)
void AOR_Pawn::bOR_pawnAccelerateOff() {
	pOR_removeFlag(AOR_PawnFlags::ACCELERATE_PAWN);
}

void AOR_Pawn::pOR_addFlag(uint8 flags) {
	pOR_flags |= flags;
}
void AOR_Pawn::pOR_removeFlag(uint8 flags) {
	pOR_flags = pOR_flags & (~flags);
}