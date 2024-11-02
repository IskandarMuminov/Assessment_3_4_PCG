#include "ProceduralInterior.h"
#include "ProceduralMeshComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "GameFramework/Actor.h"

// Sets default values for this component's properties
UProceduralInterior::UProceduralInterior()
{
    PrimaryComponentTick.bCanEverTick = true;
    GridHeight = 50.f;
    GridOffset = 1000.f;
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

        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Owner Name: %s"), *Owner->GetName()));

        
        // Ensure bounds are correctly set for object placement within the room dimensions
        TopLeft = Origin - BoxExtent;
        BottomRight = Origin + BoxExtent;
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
            // Calculate vertex location with Y offset
            FVector VertexLocation = FVector(TopLeft.X + X * VertexSpacing, TopLeft.Y + Y * VertexSpacing, GridHeight);
            Vertices.Add(VertexLocation);
            UVCoords.Add(FVector2D(X, Y));

            // Draw horizontal lines
            if (X < GridSizeX - 1) // Ensure within bounds
            {
                FVector NextVertexLocation = FVector(TopLeft.X + (X + 1) * VertexSpacing, TopLeft.Y + Y * VertexSpacing, GridHeight);
                DrawDebugLine(GetWorld(), VertexLocation, NextVertexLocation, FColor::Blue, true, -1.f, 0, 2.f);
            }

            // Draw vertical lines
            if (Y < GridSizeY - 1) // Ensure within bounds
            {
                FVector NextVertexLocation = FVector(TopLeft.X + X * VertexSpacing, TopLeft.Y + (Y + 1) * VertexSpacing, GridHeight);
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
            // Define bounds for current cell with Y offset
            FVector UpperLeft = FVector(TopLeft.X + i * VertexSpacing + ChestRadius, 
                                        TopLeft.Y + j * VertexSpacing + ChestRadius + 1000.f, GridHeight);
            FVector LowerRight = FVector(TopLeft.X + (i + 1) * VertexSpacing - ChestRadius,
                                         TopLeft.Y + (j + 1) * VertexSpacing - ChestRadius + 1000.f, GridHeight);

            // Check if we are within bounds before spawning objects
            if (i < GridSizeX - 1 && j < GridSizeY - 1)
            {
                FVector RandomChestPoint = GetRandomPointInSquare(UpperLeft, LowerRight);
                FVector RandomBarrelPoint = GetRandomPointInSquare(UpperLeft, LowerRight);
                FVector RandomBrazierPoint = GetRandomPointInSquare(UpperLeft, LowerRight);
                FVector RandomSkeletonPoint = GetRandomPointInSquare(UpperLeft, LowerRight);
                FVector RandomPotteryPoint = GetRandomPointInSquare(UpperLeft, LowerRight);

                float RandomYaw = FMath::FRandRange(0.f, 360.f);

                // Spawn objects only if there is no collision at the location
                if (!IsPointBlocked(RandomChestPoint))
                {
                    SpawnObjectAtLocation(ChestClass, RandomChestPoint, RandomYaw);
                }

                if (!IsPointBlocked(RandomBarrelPoint))
                {
                    SpawnObjectAtLocation(BarrelClass, RandomBarrelPoint, RandomYaw);
                }

                if (!IsPointBlocked(RandomBrazierPoint))
                {
                    SpawnObjectAtLocation(BrazierClass, RandomBrazierPoint, RandomYaw);
                }

                if (!IsPointBlocked(RandomSkeletonPoint))
                {
                    SpawnObjectAtLocation(SkeletonClass, RandomSkeletonPoint, RandomYaw);
                }
                if (!IsPointBlocked(RandomPotteryPoint))
                {
                    SpawnObjectAtLocation(PotteryClass, RandomPotteryPoint, RandomYaw);
                }
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
    return FVector(RandomX, RandomY, GridHeight);
}

bool UProceduralInterior::IsPointBlocked(const FVector& Point)
{
    TArray<FHitResult> HitResults;
    FCollisionQueryParams CollisionParams;
    CollisionParams.AddIgnoredActor(GetOwner()); // Ignore the parent actor

    // Perform a multi-sphere trace to check for multiple collisions around the point
    bool bHit = GetWorld()->SweepMultiByChannel(
        HitResults, Point, Point, FQuat::Identity, 
        ECC_Visibility, FCollisionShape::MakeSphere(60.f), CollisionParams);

    if (bHit)
    {
        for (const FHitResult& Hit : HitResults)
        {
            // Check if the hit component is not the "Floor"
            if (Hit.GetComponent() && Hit.GetComponent()->GetName() != "Floor")
            {
                return true; // Blocked if we hit anything other than the floor
            }
        }
    }

    return false; // Not blocked if we only hit the floor or nothing at all
}
