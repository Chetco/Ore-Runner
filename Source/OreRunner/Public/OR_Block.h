// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "OR_Block.generated.h"

enum CavernBlockType {
    BedrockBlockType,
    FloorBlockType,
    DirtBlockType,
    OreBlockType,
    LavaBlockType,
};

UCLASS()
class ORERUNNER_API AOR_Block : public AActor
{
	GENERATED_BODY()
	
    /** Dummy root component */
    UPROPERTY(Category = Block, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
    class USceneComponent* DummyRoot;

    /** StaticMesh component for the clickable block */
    UPROPERTY(Category = Block, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
    class UStaticMeshComponent* BlockMesh;

public:	
	// Sets default values for this actor's properties
	AOR_Block();
    FORCEINLINE void SetType(CavernBlockType type);

    UPROPERTY()
    class UMaterial* BedrockMaterial;

    UPROPERTY()
    class UMaterialInstance* FloorMaterial;

    UPROPERTY()
    class UMaterialInstance* DirtMaterial;

    UPROPERTY()
    class UMaterialInstance* OreMaterial;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

    CavernBlockType BlockType;

    FORCEINLINE class USceneComponent* GetDummyRoot() const { return DummyRoot; }
    FORCEINLINE class UStaticMeshComponent* GetBlockMesh() const { return BlockMesh; }
};
