// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "OR_Block.h"
#include "OR_Cavern.generated.h"

UCLASS()
class ORERUNNER_API AOR_Cavern : public AActor
{
	GENERATED_BODY()

    UPROPERTY(Category = Grid, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
    class USceneComponent* DummyRoot;
	
public:	
	// Sets default values for this actor's properties
	AOR_Cavern();

    UPROPERTY(Category = Grid, EditAnywhere, BlueprintReadOnly)
    int32 Size;

    UPROPERTY(Category = Grid, EditAnywhere, BlueprintReadOnly)
    float BlockSpacing;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

    inline void SpawnBlock(CavernBlockType type, const FVector offset);

    inline void SpawnLightSource(const FVector offset);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
