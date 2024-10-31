#include "ProceduralInterior.h"
#include "ProceduralMeshComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "GameFramework/Actor.h"

// Sets default values for this component's properties
UProceduralInterior::UProceduralInterior()
{
    PrimaryComponentTick.bCanEverTick = true;
    GridHeight = 2.f;
}

// Called when the game starts
void UProceduralInterior::BeginPlay()
{
    Super::BeginPlay();
    
    SetBoundsToParent();
    CreateGrid();
    PlaceObjectsOnGrid();
}

// Dynamically set bounds based on parent actor's size
void UProceduralInterior::SetBoundsToParent()
{
    if (AActor* Owner = GetOwner())
    {
        FVector Origin;
        FVector BoxExtent;
        
        Owner->GetActorBounds(false, Origin, BoxExtent); // Get the bounds of the parent actor
        
        // Ensure bounds are correctly set for object placement within the room dimensions
        TopLeft = Origin - BoxExtent;
        BottomRight = Origin + BoxExtent;
        
        // Log the bounds for debugging
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("TopLeft: %s, BottomRight: %s"), *TopLeft.ToString(), *BottomRight.ToString()));
    }
}

void UProceduralInterior::TickComponent(float DeltaTime, ELevelTick TickType,
    FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UProceduralInterior::CreateGrid()
{
    for (int32 Y = 0; Y < GridSizeY; Y++)
    {
        for (int32 X = 0; X < GridSizeX; X++)
        {
            // Calculate vertex location
            FVector VertexLocation = FVector(TopLeft.X + X * VertexSpacing, TopLeft.Y + Y * VertexSpacing, 0.f);
            Vertices.Add(VertexLocation);
            UVCoords.Add(FVector2D(X, Y));

            // Draw horizontal lines
            if (X < GridSizeX - 1) // Ensure within bounds
            {
                FVector NextVertexLocation = FVector(TopLeft.X + (X + 1) * VertexSpacing, TopLeft.Y + Y * VertexSpacing, 0.f);
                DrawDebugLine(GetWorld(), VertexLocation, NextVertexLocation, FColor::Blue, true, -1.f, 0, 2.f);
            }

            // Draw vertical lines
            if (Y < GridSizeY - 1) // Ensure within bounds
            {
                FVector NextVertexLocation = FVector(TopLeft.X + X * VertexSpacing, TopLeft.Y + (Y + 1) * VertexSpacing, 0.f);
                DrawDebugLine(GetWorld(), VertexLocation, NextVertexLocation, FColor::Blue, true, -1.f, 0, 2.f);
            }

            // Triangles for procedural mesh (for completeness)
            if (X < GridSizeX - 1 && Y < GridSizeY - 1)
            {
                int32 Index = Y * GridSizeX + X;
                Triangles.Add(Index);
                Triangles.Add(Index + GridSizeX);
                Triangles.Add(Index + 1);
                Triangles.Add(Index + 1);
                Triangles.Add(Index + GridSizeX);
                Triangles.Add(Index + GridSizeX + 1);
            }
        }
    }
}

void UProceduralInterior::PlaceObjectsOnGrid()
{
    // Iterate only within grid cells (no overlapping edge placement)
    for (int32 i = 0; i < GridSizeX; i++)
    {
        for (int32 j = 0; j < GridSizeY; j++)
        {
            // Define bounds for current cell
            FVector UpperLeft = FVector(TopLeft.X + i * VertexSpacing + ChestRadius, 
                                        TopLeft.Y + j * VertexSpacing + ChestRadius, GridHeight);
            FVector LowerRight = FVector(TopLeft.X + (i + 1) * VertexSpacing - ChestRadius,
                                         TopLeft.Y + (j + 1) * VertexSpacing - ChestRadius, GridHeight);

            // Check if we are within bounds before spawning objects
            if (i < GridSizeX - 1 && j < GridSizeY - 1)
            {
                FVector RandomChestPoint = GetRandomPointInSquare(UpperLeft, LowerRight);
                FVector RandomBarrelPoint = GetRandomPointInSquare(UpperLeft, LowerRight);
                FVector RandomBrazierPoint = GetRandomPointInSquare(UpperLeft, LowerRight);

                float RandomYaw = FMath::FRandRange(0.f, 360.f);

                // Spawn objects at calculated random points within the defined bounds
                SpawnObjectAtLocation(ChestClass, RandomChestPoint, RandomYaw);
                SpawnObjectAtLocation(BarrelClass, RandomBarrelPoint, RandomYaw);
                SpawnObjectAtLocation(BrazierClass, RandomBrazierPoint, RandomYaw);
            }
        }
    }
}

void UProceduralInterior::SpawnObjectAtLocation(UClass* ItemToSpawn, const FVector& Location, float Yaw)
{
    if (!ItemToSpawn) return;

    FRotator Rotation(0.f, Yaw, 0.f);
    AActor* SpawnedObject = GetWorld()->SpawnActor<AActor>(ItemToSpawn, Location, Rotation);
    if (SpawnedObject)
    {
        SpawnedObjects.Add(SpawnedObject);
    }
}

FVector UProceduralInterior::GetRandomPointInSquare(const FVector& UpperLeft, const FVector& LowerRight)
{
    float RandomX = FMath::FRandRange(UpperLeft.X, LowerRight.X);
    float RandomY = FMath::FRandRange(UpperLeft.Y, LowerRight.Y);
    return FVector(RandomX, RandomY, 0.f);
}
