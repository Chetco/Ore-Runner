// Fill out your copyright notice in the Description page of Project Settings.

#include "OR_Block.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "Materials/MaterialInstance.h"

#define LOCTEXT_NAMESPACE "OR_Block"

// Sets default values
AOR_Block::AOR_Block()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

    // Structure to hold one-time initialization
    struct FConstructorStatics
    {
        ConstructorHelpers::FObjectFinderOptional<UStaticMesh> BlockMesh;
        ConstructorHelpers::FObjectFinderOptional<UMaterial> BedrockMaterial;
        ConstructorHelpers::FObjectFinderOptional<UMaterialInstance> FloorMaterial;
        ConstructorHelpers::FObjectFinderOptional<UMaterialInstance> DirtMaterial;
        ConstructorHelpers::FObjectFinderOptional<UMaterialInstance> OreMaterial;
        FConstructorStatics()
            : BlockMesh(TEXT("/Engine/BasicShapes/Cube.Cube"))
            , BedrockMaterial(TEXT("/Game/Materials/BedrockMaterial.BedrockMaterial"))
            , FloorMaterial(TEXT("/Game/Materials/FloorMaterial.FloorMaterial"))
            , DirtMaterial(TEXT("/Game/Materials/DirtMaterial.DirtMaterial"))
            , OreMaterial(TEXT("/Game/Materials/OreMaterial.OreMaterial"))
        {
        }
    };
    static FConstructorStatics ConstructorStatics;

    // Create dummy root scene component
    DummyRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Dummy0"));
    RootComponent = DummyRoot;

    // Create static mesh component
    BlockMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BlockMesh0"));
    BlockMesh->SetStaticMesh(ConstructorStatics.BlockMesh.Get());
    BlockMesh->SetRelativeScale3D(FVector(1.f, 1.f, 1.f));
    BlockMesh->SetRelativeLocation(FVector(0.f, 0.f, 0.f));
    BlockMesh->SetMaterial(0, ConstructorStatics.BedrockMaterial.Get());
    BlockMesh->SetupAttachment(DummyRoot);

    // Save a pointer to the orange material
    BedrockMaterial = ConstructorStatics.BedrockMaterial.Get();
    FloorMaterial = ConstructorStatics.FloorMaterial.Get();
    DirtMaterial = ConstructorStatics.DirtMaterial.Get();
    OreMaterial = ConstructorStatics.OreMaterial.Get();
}

FORCEINLINE void AOR_Block::SetType(CavernBlockType type)
{
    BlockType = type;
    switch (BlockType)
    {
    case FloorBlockType:
        BlockMesh->SetMaterial(0, FloorMaterial); break;
    case DirtBlockType:
        BlockMesh->SetMaterial(0, DirtMaterial); break;
    case OreBlockType:
        BlockMesh->SetMaterial(0, OreMaterial); break;
    default:
        BlockMesh->SetMaterial(0, BedrockMaterial); break;
    }
}

// Called when the game starts or when spawned
void AOR_Block::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AOR_Block::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

