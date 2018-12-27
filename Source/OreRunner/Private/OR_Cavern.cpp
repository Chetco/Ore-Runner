// Fill out your copyright notice in the Description page of Project Settings.

#include "OR_Cavern.h"
#include "Engine/World.h"
#include "Engine/PointLight.h"

#define LOCTEXT_NAMESPACE "OR_Cavern"

// placeholder noise utility
namespace DevDad { /// github.com/devdad/SimplexNoise

/*
SimplexNoise 1.0.0
-----
DevDad - Afan Olovcic @ www.art-and-code.com - 08/12/2015
This algorithm was originally designed by Ken Perlin, but my code has been
adapted and extended from the implementation written by Stefan Gustavson (stegu@itn.liu.se)
and modified to fit to Unreal Engine 4
* This is a clean, fast, modern and free Perlin Simplex noise function.
* If we change float to double it could be even faster but there is no double type in Blueprint
* All Public Functions are BlueprintCallable so they can be used in every blueprint
From DevDad and Dedicated to you and Unreal Community
Use it free for what ever you want
I only request that you mention me in the credits for your game in the way that feels most appropriate to you.
*/

// USimplexNoiseBPLibrary
#define FASTFLOOR(x) ( ((x)>0) ? ((int)x) : (((int)x)-1) )

    unsigned char perm[512] = { 151,160,137,91,90,15,
    131,13,201,95,96,53,194,233,7,225,140,36,103,30,69,142,8,99,37,240,21,10,23,
    190, 6,148,247,120,234,75,0,26,197,62,94,252,219,203,117,35,11,32,57,177,33,
    88,237,149,56,87,174,20,125,136,171,168, 68,175,74,165,71,134,139,48,27,166,
    77,146,158,231,83,111,229,122,60,211,133,230,220,105,92,41,55,46,245,40,244,
    102,143,54, 65,25,63,161, 1,216,80,73,209,76,132,187,208, 89,18,169,200,196,
    135,130,116,188,159,86,164,100,109,198,173,186, 3,64,52,217,226,250,124,123,
    5,202,38,147,118,126,255,82,85,212,207,206,59,227,47,16,58,17,182,189,28,42,
    223,183,170,213,119,248,152, 2,44,154,163, 70,221,153,101,155,167, 43,172,9,
    129,22,39,253, 19,98,108,110,79,113,224,232,178,185, 112,104,218,246,97,228,
    251,34,242,193,238,210,144,12,191,179,162,241, 81,51,145,235,249,14,239,107,
    49,192,214, 31,181,199,106,157,184, 84,204,176,115,121,50,45,127, 4,150,254,
    138,236,205,93,222,114,67,29,24,72,243,141,128,195,78,66,215,61,156,180,
    151,160,137,91,90,15,
    131,13,201,95,96,53,194,233,7,225,140,36,103,30,69,142,8,99,37,240,21,10,23,
    190, 6,148,247,120,234,75,0,26,197,62,94,252,219,203,117,35,11,32,57,177,33,
    88,237,149,56,87,174,20,125,136,171,168, 68,175,74,165,71,134,139,48,27,166,
    77,146,158,231,83,111,229,122,60,211,133,230,220,105,92,41,55,46,245,40,244,
    102,143,54, 65,25,63,161, 1,216,80,73,209,76,132,187,208, 89,18,169,200,196,
    135,130,116,188,159,86,164,100,109,198,173,186, 3,64,52,217,226,250,124,123,
    5,202,38,147,118,126,255,82,85,212,207,206,59,227,47,16,58,17,182,189,28,42,
    223,183,170,213,119,248,152, 2,44,154,163, 70,221,153,101,155,167, 43,172,9,
    129,22,39,253, 19,98,108,110,79,113,224,232,178,185, 112,104,218,246,97,228,
    251,34,242,193,238,210,144,12,191,179,162,241, 81,51,145,235,249,14,239,107,
    49,192,214, 31,181,199,106,157,184, 84,204,176,115,121,50,45,127, 4,150,254,
    138,236,205,93,222,114,67,29,24,72,243,141,128,195,78,66,215,61,156,180
    };

    void setNoiseSeed(const int32& newSeed)
    {
        TArray<bool> availableSeeds;
        availableSeeds.Init(true, 256);
        FMath::RandInit(newSeed);
        for (uint16 it = 0; it < 256; ++it)
        {
            uint8 nextNum;
            do
            {
                nextNum = FMath::RandRange(0, 255);
            } while (!availableSeeds[nextNum]);
            DevDad::perm[it] = (unsigned char)nextNum;
            DevDad::perm[it + 256] = (unsigned char)nextNum;
        }
    }

    float grad(int hash, float x, float y)
    {
        int h = hash & 7;      // Convert low 3 bits of hash code
        float u = h < 4 ? x : y;  // into 8 simple gradient directions,
        float v = h < 4 ? y : x;  // and compute the dot product with (x,y).
        return ((h & 1) ? -u : u) + ((h & 2) ? -2.0f*v : 2.0f*v);
    }

    // 2D Simplex Noise

    float SimplexNoise2D(float x, float y)
    {
#ifndef F2
#define F2 0.366025403f // F2 = 0.5*(sqrt(3.0)-1.0)
#endif
#ifndef G2
#define G2 0.211324865f // G2 = (3.0-Math.sqrt(3.0))/6.0
#endif

        float n0, n1, n2; // Noise contributions from the three corners

                          // Skew the input space to determine which simplex cell we're in
        float s = (x + y)*F2; // Hairy factor for 2D
        float xs = x + s;
        float ys = y + s;
        int i = FASTFLOOR(xs);
        int j = FASTFLOOR(ys);

        float t = (float)(i + j)*G2;
        float X0 = i - t; // Unskew the cell origin back to (x,y) space
        float Y0 = j - t;
        float x0 = x - X0; // The x,y distances from the cell origin
        float y0 = y - Y0;

        // For the 2D case, the simplex shape is an equilateral triangle.
        // Determine which simplex we are in.
        int i1, j1; // Offsets for second (middle) corner of simplex in (i,j) coords
        if (x0 > y0) { i1 = 1; j1 = 0; } // lower triangle, XY order: (0,0)->(1,0)->(1,1)
        else { i1 = 0; j1 = 1; }      // upper triangle, YX order: (0,0)->(0,1)->(1,1)

                                      // A step of (1,0) in (i,j) means a step of (1-c,-c) in (x,y), and
                                      // a step of (0,1) in (i,j) means a step of (-c,1-c) in (x,y), where
                                      // c = (3-sqrt(3))/6

        float x1 = x0 - i1 + G2; // Offsets for middle corner in (x,y) unskewed coords
        float y1 = y0 - j1 + G2;
        float x2 = x0 - 1.0f + 2.0f * G2; // Offsets for last corner in (x,y) unskewed coords
        float y2 = y0 - 1.0f + 2.0f * G2;

        // Wrap the integer indices at 256, to avoid indexing perm[] out of bounds
        int ii = i & 0xff;
        int jj = j & 0xff;

        // Calculate the contribution from the three corners
        float t0 = 0.5f - x0 * x0 - y0 * y0;
        if (t0 < 0.0f) n0 = 0.0f;
        else {
            t0 *= t0;
            n0 = t0 * t0 * DevDad::grad(DevDad::perm[ii + perm[jj]], x0, y0);
        }

        float t1 = 0.5f - x1 * x1 - y1 * y1;
        if (t1 < 0.0f) n1 = 0.0f;
        else {
            t1 *= t1;
            n1 = t1 * t1 * DevDad::grad(DevDad::perm[ii + i1 + perm[jj + j1]], x1, y1);
        }

        float t2 = 0.5f - x2 * x2 - y2 * y2;
        if (t2 < 0.0f) n2 = 0.0f;
        else {
            t2 *= t2;
            n2 = t2 * t2 * DevDad::grad(DevDad::perm[ii + 1 + perm[jj + 1]], x2, y2);
        }

        // Add contributions from each corner to get the final noise value.
        // The result is scaled to return values in the interval [-1,1].
        //return 40.0f * (n0 + n1 + n2); // TODO: The scale factor is preliminary!	//These values currently scale from ~ [-0.884343445, 0.884343445]
        return 40.0f / 0.884343445f * (n0 + n1 + n2);	//accurate to e-9 so that values scale to [-1, 1], same acc as F2 G2.
    }

/// github.com/devdad/SimplexNoise
/// end
}

// Sets default values
AOR_Cavern::AOR_Cavern()
{
	PrimaryActorTick.bCanEverTick = false;
    DummyRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Dummy0"));
    RootComponent = DummyRoot;
}

inline void AOR_Cavern::SpawnBlock(CavernBlockType type, const FVector offset)
{
    const FVector CavernOrigin = GetActorLocation();
    const FVector BlockLocation = FVector(offset.X, offset.Y, offset.Z) + CavernOrigin;
    AOR_Block * NewBlock = GetWorld()->SpawnActor<AOR_Block>(BlockLocation, FRotator(0, 0, 0));
    NewBlock->SetType(type);
}

inline void AOR_Cavern::SpawnLightSource(const FVector offset)
{
    const FVector CavernOrigin = GetActorLocation();
    const FVector LightLocation = FVector(offset.X, offset.Y, offset.Z) + CavernOrigin;
    APointLight * NewLight = GetWorld()->SpawnActor<APointLight>(LightLocation, FRotator(0, 0, 0));
}

// Called when the game starts or when spawned
void AOR_Cavern::BeginPlay()
{
	Super::BeginPlay();
    DevDad::setNoiseSeed(42);  /// TODO : find a better seed

    const FVector CavernOrigin = GetActorLocation();

    // Number of blocks
    const int32 NumBlocks = Size * Size;

    const int32 x_BottomRow = 0;
    const int32 x_TopRow = Size * 100;
    const int32 z_GroundLevel = 0;
    const int32 z_FoundationLevel = -100;
    // Create a top and bottom boundary of bedrock blocks
    for (int32 Count = 0; Count < Size; ++Count)
    {
        // spawn bottom row ground level bedrock
        SpawnBlock(BedrockBlockType, FVector(x_BottomRow, Count * 100, z_GroundLevel));
        // spawn bottom row foundation level bedrock
        SpawnBlock(BedrockBlockType, FVector(x_BottomRow, Count * 100, z_FoundationLevel));

        // spawn top row ground level bedrock at
        SpawnBlock(BedrockBlockType, FVector(x_TopRow, Count * 100, z_GroundLevel));
        // spawn top row foundaton level bedrock
        SpawnBlock(BedrockBlockType, FVector(x_TopRow, Count * 100, z_FoundationLevel));
    }

    const int32 y_LeftColumn = 0;
    const int32 y_RightColumn = (Size - 1) * 100;

    // Create a left and right boundary of bedrock blocks
    for (int32 Count = 1; Count < Size; ++Count)
    {
        // spawn left column ground level bedrock
        SpawnBlock(BedrockBlockType, FVector(Count * 100, y_LeftColumn, z_GroundLevel));
        // spawn left column foundation level bedrock
        SpawnBlock(BedrockBlockType, FVector(Count * 100, y_LeftColumn, z_FoundationLevel));

        // spawn right column ground level bedrock
        SpawnBlock(BedrockBlockType, FVector(Count * 100, y_RightColumn, z_GroundLevel));
        // spawn right column foundaton level bedrock
        SpawnBlock(BedrockBlockType, FVector(Count * 100, y_RightColumn, z_FoundationLevel));
    }

    int32 NumInteriorBlocks = (Size - 1) * (Size - 1);
    // Create an interior of floor blocks
    for (int32 Count = 0; Count < NumInteriorBlocks - 1; ++Count)
    {
        float xpos = (Count / (Size - 2)) * 100 + 100;
        float ypos = (Count % (Size - 2)) * 100 + 100;
        float randval = DevDad::SimplexNoise2D(xpos*34.5f, ypos*34.5f);
        if (randval < 0.975)
        {
            SpawnBlock(FloorBlockType, FVector(xpos, ypos, z_FoundationLevel));
        }
        else // a hole in the foundation / trap / pitfall / cave
        {
            // do nothing / don't put a floor here
        }

    }

    static bool bPlayerStartChosen = false;

    // Create an interior of mineable blocks
    for (int32 Count = 0; Count < NumInteriorBlocks - 1; ++Count)
    {
        float xpos = (Count / (Size - 2)) * 100 + 100;
        float ypos = (Count % (Size - 2)) * 100 + 100;
        float randval = DevDad::SimplexNoise2D(xpos*34.5f,ypos*34.5f);
        //float randval = FMath::PerlinNoise1D(static_cast<float>(Count) / static_cast<float>(NumInteriorBlocks) * 1000);
        if (randval < 0.24f) // 50% chance of dirt
        {
            // dirt
            SpawnBlock(
                DirtBlockType,
                FVector(xpos, ypos, z_GroundLevel)
            );
        }
        else if (randval < 0.86f)
        {
            // open area
            if (!bPlayerStartChosen)
            {
                bPlayerStartChosen = true;
                /// TODO : spawn player pawn here?
            }
        }
        else if (randval < 0.96f)
        {
            // open area with a light
            SpawnLightSource(FVector(xpos, ypos, z_GroundLevel + 150));
        }
        else
        {
            // ore
            SpawnBlock(
                OreBlockType,
                FVector(xpos, ypos, z_GroundLevel)
            );
        }

    }
}

// Called every frame
void AOR_Cavern::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

